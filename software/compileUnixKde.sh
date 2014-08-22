#qmake QMAKE_CXXFLAGS_WARN_ON=-w QMAKE_CFLAGS_WARN_ON=-w && make
#rm main_kde.o Makefile
g++ -w -g -Wall -Iinclude -Isrc/kiss_fft130 src/kiss_fft130/kiss_fft.c src/music_kde.cpp -o bin/MusicLight -Llib -lboost_system -lpulse -lpulse-simple
