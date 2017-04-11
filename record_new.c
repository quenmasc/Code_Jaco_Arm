/*
 * Simple audio test capture to buffer
 *
 * Author: Liam Girdwood
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITH ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include <alsa/asoundlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <signal.h>

#define APP_PERIOD_SIZE	2048
#define APP_NUM_PERIODS	256
#define DRIVER_PERIODS	3
#define WATERMARK	2

static int debug_avail_per[APP_NUM_PERIODS];
static int complete = 0, wperiods = 0, rperiods = 0;

struct alsa_source {
	snd_pcm_t *handle;
	
	/* from cmd line parameters */
	snd_pcm_format_t format;
	unsigned int channels;
	unsigned int rate;
	unsigned int period_bytes;  /* bytes in a period */
	
	/* derived from cmd line params */
	snd_pcm_uframes_t period_frames; /* frames in a period */
	int bits_per_sample;
	int bits_per_frame;
	
	/* from sound hardware */
	size_t buffer_size;
	snd_pcm_uframes_t buffer_frames;  /* size of buffer in frames */

	/* buffer data */
	char *buffer;
	int reader_position; /* in bytes */
	int writer_position;

	/* locking/threading */
	pthread_t tid;
	pthread_mutex_t mutex; /* for access to reader/writer pos */
	sem_t sem; /* for waking writer */

	/* out file */
	int out_fd;
};

/* for debug logging */
static snd_output_t *log;

static int dump_control(snd_ctl_t *handle, snd_ctl_elem_id_t *id)
{
	int err, count, i;
	snd_ctl_elem_info_t *info;
	snd_ctl_elem_type_t type;
	snd_ctl_elem_value_t *control;

	/* alloc mem */
	snd_ctl_elem_info_alloca(&info);
	snd_ctl_elem_value_alloca(&control);

	/* get info for control */
	snd_ctl_elem_info_set_id(info, id);
	err = snd_ctl_elem_info(handle, info);
	if (err < 0) {
		printf("failed to get ctl info\n");
		return err;
	}

	/* read the control */
	snd_ctl_elem_value_set_id(control, id);
	snd_ctl_elem_read(handle, control);

	/* get type and channels for control */
	type = snd_ctl_elem_info_get_type(info);
	count = snd_ctl_elem_info_get_count(info);
	if (count == 0)
		return 0;

	printf("%u:'%s':%d:",
	       snd_ctl_elem_id_get_numid(id),
	       snd_ctl_elem_id_get_name(id), count);

	switch (type) {
	case SND_CTL_ELEM_TYPE_BOOLEAN:
		for (i = 0; i < count - 1; i++)
			printf("%d,",
				snd_ctl_elem_value_get_boolean(control, i));
		printf("%d", snd_ctl_elem_value_get_boolean(control, i));
		break;
	case SND_CTL_ELEM_TYPE_INTEGER:
		for (i = 0; i < count - 1; i++)
			printf("%ld,",
				snd_ctl_elem_value_get_integer(control, i));
		printf("%ld", snd_ctl_elem_value_get_integer(control, i));
		break;
	case SND_CTL_ELEM_TYPE_INTEGER64:
		for (i = 0; i < count - 1; i++)
			printf("%lld,",
				snd_ctl_elem_value_get_integer64(control, i));
		printf("%lld",
				snd_ctl_elem_value_get_integer64(control, i));
		break;
	case SND_CTL_ELEM_TYPE_ENUMERATED:
		for (i = 0; i < count - 1; i++)
			printf("%d,",
				snd_ctl_elem_value_get_enumerated(control, i));
		printf("%d",
				snd_ctl_elem_value_get_enumerated(control, i));
		break;
	case SND_CTL_ELEM_TYPE_BYTES:
		for (i = 0; i < count - 1; i++)
			printf("%2.2x,",
				snd_ctl_elem_value_get_byte(control, i));
		printf("%2.2x", snd_ctl_elem_value_get_byte(control, i));
		break;
	default:
		break;
	}
	printf("\n");
	return 0;
}


static int mixers_display(snd_output_t *output, const char *card_name)
{
	snd_ctl_t *handle;
	snd_ctl_card_info_t *info;
	snd_ctl_elem_list_t *list;
	int ret, i, count;

	snd_ctl_card_info_alloca(&info);
	snd_ctl_elem_list_alloca(&list);

	/* open and load snd card */
	ret = snd_ctl_open(&handle, card_name, SND_CTL_READONLY);
	if (ret < 0) {
		printf("%s: control %s open retor: %s\n", __func__, card_name,
			snd_strerror(ret));
		return ret;
	}

	/* get sound card info */
	ret = snd_ctl_card_info(handle, info);
	if (ret < 0) {
		printf("%s :control %s local retor: %s\n", __func__,
			card_name, snd_strerror(ret));
		goto close;
	}

	/* get the mixer element list */
	ret = snd_ctl_elem_list(handle, list);
	if (ret < 0) {
		printf("%s: cannot determine controls: %s\n", __func__,
			snd_strerror(ret));
		goto close;
	}

	/* get number of elements */
	count = snd_ctl_elem_list_get_count(list);
	if (count < 0) {
		ret = 0;
		goto close;
	}

	/* alloc space for elements */
	snd_ctl_elem_list_set_offset(list, 0);
	if (snd_ctl_elem_list_alloc_space(list, count) < 0) {
		printf("%s: not enough memory...\n", __func__);
		ret =  -ENOMEM;
		goto close;
	}

	/* get list of elements */
	if ((ret = snd_ctl_elem_list(handle, list)) < 0) {
		printf("%s: cannot determine controls: %s\n", __func__,
			snd_strerror(ret));
		goto free;
	}

	/* iterate through list for each kcontrol and display */
	for (i = 0; i < count; ++i) {
		snd_ctl_elem_id_t *id;
		snd_ctl_elem_id_alloca(&id);
		snd_ctl_elem_list_get_id(list, i, id);

		ret = dump_control(handle, id);
		if (ret < 0) {
			printf("%s: cannot determine controls: %s\n",
				__func__, snd_strerror(ret));
			goto free;
		}
	}
free:
	snd_ctl_elem_list_free_space(list);
close:
	snd_ctl_close(handle);
	return ret;
}

/* write PCM data to output file  - does not add WAV header */
static void *writer_thread(void *data)
{
	struct alsa_source *alsa = (struct alsa_source*)data;
	char * buf = alsa->buffer + alsa->writer_position;
	int bytes, overlap, val;
	
	while (1) {
		/* wait for next frame to be avail */
		sem_wait(&alsa->sem);

		/* pointer wrap ? */
		if (alsa->writer_position == alsa->period_bytes * APP_NUM_PERIODS) {

			alsa->writer_position = 0;
			buf = alsa->buffer;
		}

//		printf("%s: write 0x%x at offset 0x%x \n", __func__,
//			alsa->period_bytes, alsa->writer_position);

		/* write whole period */
		bytes = write(alsa->out_fd, buf, alsa->period_bytes);
		wperiods++;

		/* inc writer pos */
		alsa->writer_position += bytes;

//		printf("%s: new write offset 0x%x \n", __func__,
//			alsa->writer_position);
		buf = alsa->buffer + alsa->writer_position;

		/* check how many periods are waiting to be written to file */
		sem_getvalue(&alsa->sem, &val);
		/* has the writer been overtaken by the reader ??? */
		if (val >= APP_NUM_PERIODS)
			printf("w: overwrite %d\n",val);
		if (val < APP_NUM_PERIODS)
			debug_avail_per[val]++;
		if (val == 0 && complete == 1)
			goto out;
	}

out:
	printf("writer finished %d periods\n", wperiods);
	return NULL;
}

/* sets the audio parameters */
static int alsa_set_params(struct alsa_source *alsa_data)
{
	snd_pcm_hw_params_t *params;
	snd_pcm_sw_params_t *swparams;
	int err;
	
	snd_pcm_hw_params_alloca(&params);
	snd_pcm_sw_params_alloca(&swparams);
	
	/* number of bits per sample e.g. 16 for S16_LE */
	alsa_data->bits_per_sample =
			snd_pcm_format_physical_width(alsa_data->format);

	/* number of bits in alsa frame e.g. 32 for S16_LE stereo */
	alsa_data->bits_per_frame =
		alsa_data->bits_per_sample * alsa_data->channels;

	/* number of frames in a period (period bytes / frame bytes) */
	alsa_data->period_frames = 
		alsa_data->period_bytes / (alsa_data->bits_per_frame >> 3);

	/* config Hardware params */
	err = snd_pcm_hw_params_any(alsa_data->handle, params);
	if (err < 0) {
		printf("Broken configuration for this PCM\n");
		return -ENODEV;
	}
	
	/* set pcm format to be interleaved (e.g. LRLRLR for stereo) */
	err = snd_pcm_hw_params_set_access(alsa_data->handle, params,
						SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err < 0) {
		printf("Access type not available\n");
		return -EINVAL;
	}
	
	/* sample format e.g. 16 bit little endian S16_LE */
	err = snd_pcm_hw_params_set_format(alsa_data->handle, params,
						alsa_data->format);
	if (err < 0) {
		printf("Sample format non available\n");
		return -EINVAL;
	}
	
	/* number of channels */
	err = snd_pcm_hw_params_set_channels(alsa_data->handle, params,
						alsa_data->channels);
	if (err < 0) {
		printf("Channels count non available\n");
		return -EINVAL;
	}

	/* rate (or nearest) */
	err = snd_pcm_hw_params_set_rate_near(alsa_data->handle, params,
						&alsa_data->rate, 0);
	if (err < 0) {
		printf("rate non available\n");
		return -EINVAL;
	}

	/* config hardware buffering */

	/* get max supported buffer size */
	err = snd_pcm_hw_params_get_buffer_size_max(params, 
						    &alsa_data->buffer_frames);

	/* we want buffer to be atleast DRIVER_PERIODS in size */
	if ((alsa_data->buffer_frames / alsa_data->period_frames) < DRIVER_PERIODS) {
		printf("ALSA buffer too small %ld frames need %ld\n",
		       alsa_data->buffer_frames, 
		       alsa_data->period_frames * (DRIVER_PERIODS + 1));
		return -EINVAL;
	}

	/* set required period size */
	err = snd_pcm_hw_params_set_period_size(alsa_data->handle, params,
						alsa_data->period_frames, 0);
	if (err < 0) {
		printf("period size not available\n");
		return -EINVAL;
	}

	/* set required buffer size (or nearest)*/
	err = snd_pcm_hw_params_set_buffer_size_near(alsa_data->handle, params,
							     &alsa_data->buffer_frames);
	if (err < 0) {
		printf("buffer size not available\n");
		return -EINVAL;
	}
	
	/* commit all above hardware audio parameters to driver */
	err = snd_pcm_hw_params(alsa_data->handle, params);
	if (err < 0) {
		printf("Unable to install hw params\n");
		return -EINVAL;
	}

	/* config software audio params */
	snd_pcm_sw_params_current(alsa_data->handle, swparams);

	err = snd_pcm_sw_params_set_avail_min(alsa_data->handle, swparams,
						alsa_data->period_frames);
	if (err < 0) {
		printf("failed to set avail min\n");
		return -EINVAL;
	}
	
	/* frames for alsa-lib/driver to buffer internally before starting */ 
	err = snd_pcm_sw_params_set_start_threshold(alsa_data->handle,
							swparams, 1);
	if (err < 0) {
		printf("failed to set start threshold\n");
		return -EINVAL;
	}
	
	/* if free frames >= buffer frames then stop */
	err = snd_pcm_sw_params_set_stop_threshold(alsa_data->handle, swparams,
							alsa_data->buffer_frames);
	if (err < 0) {
		printf("failed to set stop threshold\n");
		return -EINVAL;
	}

	/* commit the software params to alsa-lib */
	if (snd_pcm_sw_params(alsa_data->handle, swparams) < 0) {
		printf("unable to install sw params\n");
		snd_pcm_sw_params_dump(swparams, log);
		return -EINVAL;
	}

	snd_pcm_dump(alsa_data->handle, log);
	return 0;
}

/* read pcm data from the audio driver */
static ssize_t alsa_pcm_read(struct alsa_source *alsa, size_t count)
{
	char *data = alsa->buffer + alsa->reader_position;
	ssize_t size;
	ssize_t result = 0;
	snd_pcm_uframes_t frames;
	int wait, val, overwrite = 0;
	
	/* check how many periods are free for reader */
	sem_getvalue(&alsa->sem, &val);

	/* has we overtaken the writer ?? */
	if (val >= APP_NUM_PERIODS - 1) {
		/*
		 * We have many choices here (depending on our policy):-
		 * 1. Restart capture.
		 * 2. Discard current period and wait for writer to catch up.
		 * 3. Discard/flush writer periods.
		 * We choose option 2 in this example.
		 */
		printf("r: overwrite %d\n",val);
		overwrite = 1;
	}

	/* change count from bytes to frames */
	frames = count / (alsa->bits_per_frame >> 3);
//	printf("%s: need 0x%x frames count 0x%x \n", __func__, frames, count);

	/* read a number of frames from the driver */
	while (frames > 0) {
		size = snd_pcm_readi(alsa->handle, data, frames);
		
		if (size == -EAGAIN || (size >= 0 && (size_t)size < frames)) {
			/* wait 1000ms max */
			wait = snd_pcm_wait(alsa->handle, 1000);
			if (wait <= 0)
				printf("wait timeout/error\n");
		} else if (size == -EPIPE) {
			printf("xrun();\n");
		} else if (size == -ESTRPIPE) {
			printf("suspend();\n");
		} else if (size < 0) {
			printf("read error: %s", snd_strerror(size));
		}
		
		/* still have data to read ? */
		if (size > 0) {
			result += size;
			frames -= size;
			data += size * alsa->bits_per_frame / 8;
		}
	}

	/* return if we are about to overwrrite */
	if (overwrite)
		return result;
	
	/* got frames, so update reader pointer */
	alsa->reader_position += count;
//	printf("%s: buffer pos 0x%x size 0x%x\n", __func__, alsa->reader_position,
//		alsa->period_bytes * APP_NUM_PERIODS);

	/* buffer wrap ? */
	if (alsa->reader_position >= alsa->period_bytes * APP_NUM_PERIODS)
		alsa->reader_position = 0;

	/* tel writer we have frame */
	sem_post(&alsa->sem);
	rperiods++;

	return result;
}

/* CTRL - C handler - dump used period debug values */
static void shutdown(int sig)
{
	int i;

	/* dump debug */
	for (i = 0; i < APP_NUM_PERIODS; i++)
		printf("per %d val %d\n", i, debug_avail_per[i]);
	
	complete = 1;
}

int main(int argc, char *argv[])
{
	struct alsa_source *alsa_data;
	int ret, channels = 2;
	char out_file[32];

	if (argc == 2)
		channels = atoi(argv[1]);
	if (channels < 1 || channels > 9) {
		printf("%s: channels must be 1 - 8\n",argv[0]);
		exit(0);
	}

	printf("%s recording %d chans using %d bytes\n", argv[0], channels,
		APP_PERIOD_SIZE * APP_NUM_PERIODS * channels);
	
	sprintf(out_file, "out-%d.raw",channels); 

	alsa_data = malloc(sizeof(struct alsa_source));
	if (alsa_data == NULL)
		return -ENOMEM;
	
	alsa_data->buffer = malloc(APP_PERIOD_SIZE * APP_NUM_PERIODS * channels);
	if (alsa_data->buffer == NULL)
		return -ENOMEM;

	/* todo - configure via cmd line */
	alsa_data->channels = channels;
	alsa_data->rate = 48000;
	alsa_data->format = SND_PCM_FORMAT_S16_LE;
	alsa_data->period_bytes = APP_PERIOD_SIZE * channels;
	alsa_data->reader_position = 0;
	alsa_data->writer_position = 0;
	
	/* for debug */
	ret = snd_output_stdio_attach(&log, stderr, 0);
	bzero(debug_avail_per, APP_NUM_PERIODS * sizeof(int));

	/* display controls */
	mixers_display(log, "default");
	
	/* catch ctrl C and dump buffer info */
	signal(SIGINT, shutdown);

	/* the output file */
	alsa_data->out_fd = open(out_file, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	if (alsa_data->out_fd < 0) {
		printf("failed to open %s err %d\n",
			out_file, alsa_data->out_fd);
		return alsa_data->out_fd;
	}

	/* open the alsa source */
	ret = snd_pcm_open(&alsa_data->handle, "hw:0,0", SND_PCM_STREAM_CAPTURE, 0);
	if (ret < 0) {
		printf("audio open error: %s\n", snd_strerror(ret));
		return ret;
	}
	
	/* configure audio */
	ret = alsa_set_params(alsa_data);
	if (ret < 0) {
		printf("set params error: %d\n", ret);
		return ret;
	}
	
	/* init threads */
	pthread_mutex_init(&alsa_data->mutex, NULL);
	sem_init(&alsa_data->sem, 0, 0);
	ret = pthread_create(&alsa_data->tid, NULL, writer_thread, alsa_data);
	if (ret < 0) {
		printf("failed to create thread %d\n", ret);
		return ret;
	}

	/* todo - exit the loop after n iterations */
	while (!complete) {
		alsa_pcm_read(alsa_data, APP_PERIOD_SIZE * channels);
	}
	/* this may be different from writer periods depending where in
	   the read/write cycle ctrl-C was pressed */
	printf("reader finished %d periods\n", rperiods);

	/* wait for writer to finish */
	pthread_join(alsa_data->tid, NULL);

	/* cleanup */
	snd_pcm_nonblock(alsa_data->handle, 0);
	snd_pcm_drain(alsa_data->handle);
	snd_pcm_close(alsa_data->handle);
	snd_output_close(log);
	return 0;
}

