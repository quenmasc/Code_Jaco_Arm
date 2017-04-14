import alsaaudio
from multiprocessing import Process, Queue
import numpy as np
import struct
import array
import time
"""
A class implementing buffered audio I/O.
"""
class Audio:
    """
    Initialize the audio buffer.
    """
    def __init__(self):
        #self.__rate = 96000
        self.__rate = 8000
        self.__stride = 4
        self.__pre_post = 4
        self.__read_queue = Queue()
        self.__write_queue = Queue()

    """
    Reads audio from an ALSA audio device into the read queue.
    Supposed to run in its own process.
    """
    def __read(self):
        card='sysdefault:CARD=Device'
        inp = alsaaudio.PCM(alsaaudio.PCM_CAPTURE, alsaaudio.PCM_NORMAL,card)
        inp.setchannels(1)
        inp.setrate(self.__rate)
        inp.setformat(alsaaudio.PCM_FORMAT_S16_LE)
        inp.setperiodsize(self.__rate / 50)

        while True:
            frame_c, data = inp.read()
            self.__read_queue.put(data)

    """
    Writes audio to an ALSA audio device from the write queue.
    Supposed to run in its own process.
    """
    def __write(self):
        card='sysdefault:CARD=Device'
        outp = alsaaudio.PCM(alsaaudio.PCM_PLAYBACK, alsaaudio.PCM_NORMAL,card)
        outp.setchannels(1)
        outp.setrate(self.__rate)
        outp.setformat(alsaaudio.PCM_FORMAT_S32_LE)
        outp.setperiodsize(self.__rate / 100)

        while True:
            data = self.__write_queue.get()
            outp.write(data)

    """
    Pre-post data into the output buffer to avoid buffer underrun.
    """
    def __pre_post_data(self):
        zeros = np.zeros(self.__rate / 50, dtype = np.uint32)

        for i in range(0, self.__pre_post):
            self.__write_queue.put(zeros)

    """
    Runs the read and write processes.
    """
    def run(self):
        self.__pre_post_data()
        read_process = Process(target = self.__read)
        write_process = Process(target = self.__write)
        read_process.start()
        write_process.start()

    """
    Reads audio samples from the queue captured from the reading thread.
    """
    def read(self):
        return self.__read_queue.get()

    """
    Writes audio samples to the queue to be played by the writing thread.
    """
    def write(self, data):
        self.__write_queue.put(data)

    """
    Pseudonymize the audio samples from a binary string into an array of integers.
    """
    def pseudonymize(self, s):
        return struct.unpack(">" + ("I" * (len(s) / self.__stride)), s)

    """
    Depseudonymize the audio samples from an array of integers into a binary string.
    """
    def depseudonymize(self, a):
        s = ""

        for elem in a:
            s += struct.pack(">I", elem)

        return s

    """
    Normalize the audio samples from an array of integers into an array of floats with unity level.
    """
    def normalize(self, data, max_val):
        data = np.array(data)
        bias = int(0.5 * max_val)
        fac = 1.0 / (0.5 * max_val)
        data = fac * (data - bias)
        return data

    """
    Denormalize the data from an array of floats with unity level into an array of integers.
    """
    def denormalize(self, data, max_val):
        bias = int(0.5 * max_val)
        fac = 0.5 * max_val
        data = np.array(data)
        data = (fac * data).astype(np.int64) + bias
        return data

debug = True
audio = Audio()
audio.run()
j=0
while True:
    raw_data=b''
    start=time.time()
    while len(raw_data)<2*8000:
        data = audio.read()
        raw_data+=data
    end=time.time()
    print(len(np.fromstring(raw_data[:2*8000],dtype=np.int16)))
    print(np.fromstring(raw_data[:2*8000],dtype=np.int16))
    print(end-start)
    #  if frame_count > 0:
  ##  data_float_array=array.array('f',np.fromstring(data,dtype=np.float32))
   # j+=1
    #time.sleep(0.0001)
   # if j>=15 :
    #    print(data_float_array)
     #   break
#    pdata = audio.pseudonymize(data)
    
 #  if debug:
#        print "[PRE-PSEUDONYMIZED] Min: " + str(np.min(pdata)) + ", Max: " + str(np.max(pdata))

 #   ndata = audio.normalize(pdata, 0xffffffff)

  #  if debug:
  #      print "[PRE-NORMALIZED] Min: " + str(np.min(ndata)) + ", Max: " + str(np.max(ndata))
  #      print "[PRE-NORMALIZED] Level: " + str(int(10.0 * np.log10(np.max(np.absolute(ndata)))))

    #ndata += 0.01 # When I comment in this line, it wreaks complete havoc!

  #  if debug:
   #     print "[POST-NORMALIZED] Level: " + str(int(10.0 * np.log10(np.max(np.absolute(ndata)))))
  #      print "[POST-NORMALIZED] Min: " + str(np.min(ndata)) + ", Max: " + str(np.max(ndata))

#    pdata = audio.denormalize(ndata, 0xffffffff)

  #  if debug:
  #      print "[POST-PSEUDONYMIZED] Min: " + str(np.min(pdata)) + ", Max: " + str(np.max(pdata))
  #      print 

  #  data = audio.depseudonymize(pdata)
  #  audio.write(data)
