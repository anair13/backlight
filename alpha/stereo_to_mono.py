import wave

source = "music/fiddle.wav"
destination = source[:-4] + "_mono.wav"

source, destination = wave.open(source, "r"), wave.open(destination, "w")

destination.setnchannels(1)
destination.setsampwidth(source.getsampwidth())
destination.setframerate(source.getframerate())
destination.setcomptype(source.getcomptype(),source.getcompname())
destination.writeframes(source.readframes(source.getnframes()))

destination.close()
source.close()
