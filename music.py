from scikits.audiolab import *
from pylab import *
import numpy
import pygame
from copy import *
import math
from time import sleep
import arduino

def get_millis(position, fs = 44100):
	return position / fs * 1000

def get_pos(millis, fs = 44100):
	return millis * fs / 1000

def music_to_color(freq, amp):
	"""
	Takes a frequency and amplitude(normalized) and converts
	to an an HSV and then to an RGB byte code to be outputted to the
	arduino. This is Saavan's contribution (other than hardware)
	Frequency should be 20Hz to 20kHz
	Amplitude should be 0 to 1
	"""
	#ln 20 = 3 ln 20 000 = 9.9
	hue = math.log1p(freq)-3
	hue = hue/7 * 360
	return hsv2rgb(hue, 1, amp)


def hsv2rgb(h, s, v):
    h = float(h)
    s = float(s)
    v = float(v)
    h60 = h / 60.0
    h60f = math.floor(h60)
    hi = int(h60f) % 6
    f = h60 - h60f
    p = v * (1 - s)
    q = v * (1 - f * s)
    t = v * (1 - (1 - f) * s)
    r, g, b = 0, 0, 0
    if hi == 0: r, g, b = v, t, p
    elif hi == 1: r, g, b = q, v, p
    elif hi == 2: r, g, b = p, v, t
    elif hi == 3: r, g, b = p, q, v
    elif hi == 4: r, g, b = t, p, v
    elif hi == 5: r, g, b = v, p, q
    r, g, b = int(r * 255), int(g * 255), int(b * 255)
    return r, g, b
    
def rgb2hsv(r, g, b):
    r, g, b = r/255.0, g/255.0, b/255.0
    mx = max(r, g, b)
    mn = min(r, g, b)
    df = mx-mn
    if mx == mn:
        h = 0
    elif mx == r:
        h = (60 * ((g-b)/df) + 360) % 360
    elif mx == g:
        h = (60 * ((b-r)/df) + 120) % 360
    elif mx == b:
        h = (60 * ((r-g)/df) + 240) % 360
    if mx == 0:
        s = 0
    else:
        s = df/mx
    v = mx
    return h, s, v

def most_common(lst):
    return max(set(lst), key=lst.count)

filename = "music/beethoven5.wav"
#filename = "music/guitarc.wav"
#filename = "music/cscale_mono.wav"
#filename = "music/fiddle_mono.wav"
#filename = "music/dubstep/Bass/DS_BassB140D-03.wav"

pygame.init()
pygame.mixer.init()
pygame.mixer.music.load(filename)

sound_file = Sndfile(filename, 'r')
signal = sound_file.read_frames(sound_file.nframes)
sound_length = 1000.0 * sound_file.nframes / sound_file.samplerate # in millis

N = 4096
S, freqs, t, im = specgram(signal, N, 44100, scale_by_freq = False) #, window=numpy.hanning(N))
S = S.transpose()

max_f_values = [t.max() for t in S]
maxval = max(max_f_values)
max_f_indices = [t.argmax() for t in S]
max_f = zip(max_f_indices, max_f_values)
spect_length = len(max_f)

# fund_freqs = []
# for t in S:
# 	bins = []
# 	for i in range(1, len(t) // 10):
# 		print numpy.mean(t[:10000:i]),
# 		bins.append(numpy.mean(t[:10000:i]))
# 	#print(bins)
# 	fund_freqs.append(freqs[bins.index(max(bins))])

# print(fund_freqs)

show()

pygame.mixer.music.play()
i = 00
playpos = 0
while playpos >= 0:
	# fft
	playpos = pygame.mixer.music.get_pos()
	t = int(min(max(0, playpos * spect_length / sound_length), spect_length - 1))
	f = max_f[t]
	freq = f[0]
	amp = f[1]
	fund_freq = freqs[freq]
	mag = math.log(amp / maxval + 1, 2) 
	r, g, b = music_to_color(fund_freq, mag)
	arduino.sendColor(r, g, b)
	sleep(0.1)
	print(playpos, fund_freq, mag)