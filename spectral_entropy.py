# Copyright (c) 2017 Laval University
#
# You may copy and modify this freely under the same terms

__author__ = "Quentin Mascret <quentin.mascret.1@ulaval.ca>"
__version__ = "1.0.0-dev"
__date__ ="07.04.2017"

import numpy as np
import numpy.fft
from scipy import signal


class SPECTRAL_ENTROPY(object):
    def __init__(self, wlen=0.015, wshift=0.005, samplerate=16000,
                 nfft=256, p=0.96, alpha=0.96):
        # store parameter
        self.wlen = int(wlen * samplerate)
        self.wshift = int(wshift * samplerate)
        self.p = p
        self.nfft = nfft
        self.samplerate = samplerate

        # build hanning window
        self.win = numpy.hamming(self.wlen)

        # prior sample for preemphasis
        self.alpha = alpha
        self.prior = 0

    def pre_emphasis(self, frame):

        """Pre-emphasis signal in order to"""
        outfr = numpy.empty(len(frame), 'd')
        outfr[0] = frame[0] - self.alpha * self.prior
        for i in range(1, len(frame)):
            outfr[i] = frame[i] - self.alpha * frame[i - 1]
        self.prior = frame[-1]
        return outfr

    def frame2periodogram(self, frame):

        """Calculate Power Spectral Density of the frame via squaring its amplitude and 
                                                                            normalizing by the numbers of bins"""
        f, Pxx_den = signal.periodogram(self.pre_emphasis(frame), self.samplerate, self.win, self.nfft,
                                        detrend='constant', return_onesided=True, scaling='density', axis=-1)

        # Normalize the calculated PSD do that it can be viewed as a probability Density function (equal to 1)
        Normalize_PSD = Pxx_den / np.sum(Pxx_den)

        # Power Spectral Entropy
        eps = 2.220446049250313e-16
        log_p = np.log2(Normalize_PSD+eps)
        p=Normalize_PSD+eps
        print(-np.sum(p * log_p))
        return -np.sum(p * log_p)

    def euclideandistance(self, frame_noise,frame):
        """ Calculate the difference between background entropy value and current entropy value"""
        return self.frame2periodogram(frame)-frame_noise
         # need to ask to Ulysse why I have a mistake when I print value first

