qmake QMAKE_CXXFLAGS_WARN_ON=-w QMAKE_CFLAGS_WARN_ON=-w && make
rm main_kde.o Makefile
mv main_kde BearHack
