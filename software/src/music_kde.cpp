#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include "HSV.h"
#include "SerialHandlerUnix.h"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <time.h>
#include <algorithm>
#include "kiss_fft.h"

typedef unsigned char BYTE;

struct Color {
	BYTE r, g, b;
	Color(BYTE r, BYTE g, BYTE b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
};

void init();
void deinit();
void writeToBuffer(uint8_t* buf);
void delay(int ms);
void decay(uint8_t* buf, unsigned int size);
void highpass(uint8_t* buf, unsigned int size);

const int BAUD_RATE = 9600;
std::string PORT = "/dev/ttyACM0";
const int BUF_SIZE = 256;

pa_simple *s;

void init(int argc, char** argv) {
    pa_sample_spec ss;
    ss.format = PA_SAMPLE_S16LE;
    ss.channels = 2;
    ss.rate = 44100;
    if (!(s = pa_simple_new(NULL, // Use the default server.
                      "Music Light", // Our application's name.
                      PA_STREAM_RECORD,
                      NULL, // Use the default device.
                      "Music Light", // Description of our stream.
                      &ss, // Our sample format.
                      NULL, // Use default channel map
                      NULL, // Use default buffering attributes.
                      NULL // Ignore error code.
    ))) {
        std::cout << "Error calling pa_simple_new" << std::endl;
    }
}
void deinit() {
    pa_simple_free(s);
}
void writeToBuffer(uint8_t* buf) {
    int error;
    if (pa_simple_read(s, buf, BUF_SIZE, &error) < 0) {
        std::cout << "Error reading: " << pa_strerror(error) << std::endl;
    }
}
void delay(int ms) {
    clock_t goal = ms * CLOCKS_PER_SEC / 1000 + clock();
    while(goal > clock());
}

int main(int argc, char** argv) {
    init(argc, argv);
    SerialHandler serialHandler(PORT, BAUD_RATE);
    
    const int OLD_BUF_SIZE = 16;
    unsigned int oldBuf[OLD_BUF_SIZE];
    unsigned int oldBufInd = 0;

    for (int i = 0; i < OLD_BUF_SIZE; i++) {
        oldBuf[i] = 0;
    }
    
    while(true) {
        uint8_t buf[BUF_SIZE];
        writeToBuffer(buf);
        decay(buf, BUF_SIZE);
        //highpass(buf, BUF_SIZE);

        unsigned int avg = 0;
        for (int i = 0; i < BUF_SIZE; i += 2) {
            //avg += buf[i];
            double v = pow((double)1.06, buf[i]) * 255 / 600000;
            if (v > 255) v = 255;
            avg += v;
            /*
            avg += pow((double)1.06, buf[i]) * 255 / 2837891;
            unsigned int v;
            v = (pow((double)1.06, buf[i]) / 2837891) * 255;
            if (v > max) {
                max = v;
            }
            */
        }
        avg /= BUF_SIZE / 2;

        oldBuf[oldBufInd] = avg;
        oldBufInd++;
        if (oldBufInd >= OLD_BUF_SIZE) {
            oldBufInd = 0;
        }

        unsigned int max = 0;
        unsigned int min = UINT_MAX;

        unsigned int smoothAvg = 0;
        for (int i = 0; i < OLD_BUF_SIZE; i++) {
            unsigned int v = oldBuf[i];
            if (v < min)
                min = v;
            if (v > max)
                max = v;
            smoothAvg += oldBuf[i];
        }
        smoothAvg /= OLD_BUF_SIZE;
        smoothAvg = std::min(255, std::max(0, 128 - (128 - (int)smoothAvg) * 3));
        smoothAvg *= ((float) (avg - min) * 255 / (max - min + 1)) / 255;
        std::cout << std::string(smoothAvg / 2, 'X') << "     " << smoothAvg << std::endl;
        int r, g, b = 0;
        int h, s, v = 0;
        h = (unsigned int) (((float)clock() / CLOCKS_PER_SEC) * 500) & 0xFF;
        s = 255;
        v = smoothAvg;
        HSVtoRGB(&r, &g, &b, h, s, v);
        serialHandler.writeSerial(r, g, b);
    }
    deinit();
    return 0;
}

void decay(uint8_t* buf, unsigned int size) {
    const int DECAY = 20;
    for (int i = 1; i < size; i++) {
        if (buf[i] < buf[i-1]) {
            if (buf[i-1] >= DECAY) {
                buf[i] = buf[i-1] - DECAY;
            } else {
                buf[i] = 0;
            }
        }
    }
}

void highpass(uint8_t* buf, unsigned int size) {
    kiss_fft_cfg cfg = kiss_fft_alloc(size, 0, NULL, NULL);
    kiss_fft_cfg icfg = kiss_fft_alloc(size, 1, NULL, NULL);

    kiss_fft_cpx fftBuf[size];
    for (unsigned int i = 0; i < size; i++) {
        fftBuf[i].r = buf[i];
        fftBuf[i].i = 0;
    }

    kiss_fft(cfg, fftBuf, fftBuf);
    for (unsigned int i = 1; i < (size / 2) - 10; i++) {
        fftBuf[i].r = 0;
        fftBuf[i].i = 0;
        fftBuf[size - i - 1].r = 0;
        fftBuf[size - i - 1].i = 0;
    }

    kiss_fft(icfg, fftBuf, fftBuf);

    for (unsigned int i = 0; i < size; i++) {
        buf[i] = fftBuf[i].r * 6.28;
    }
}
#endif
