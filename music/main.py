from scikits.audiolab import *
from pylab import *

#sound_file = Sndfile('files/beethoven5.wav', 'r')
sound_file = Sndfile('files/guitarc.wav', 'r')
signal = sound_file.read_frames(sound_file.nframes)
#signal = sound_file.read_frames(1000000)
specgram(signal)
show()