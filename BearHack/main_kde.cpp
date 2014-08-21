#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include "HSV.h"

const int W = 200;
const int H = 200;

typedef unsigned char BYTE;
int SCR_W;
int SCR_H;

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
void updatePixels();
Color getPixel(int x, int y);
void delay(int ms);

const int BAUD_RATE = 9600;

#include "SerialHandlerUnix.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPixmap>
#include <time.h>

std::string PORT = "/dev/ttyACM0";
QApplication* app;
QImage* winIm;

void init(int argc, char** argv) {
    app = new QApplication(argc, argv);
    winIm = new QImage();
}
void deinit() {
}
void updatePixels() {
    delete winIm;
    winIm = new QImage();
    *winIm = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();
    SCR_W = winIm->width();
    SCR_H = winIm->height();
}
Color getPixel(int x, int y) {
    QRgb rgb = winIm->pixel(x, y);
    QColor c = QColor(rgb);
    return Color(c.red(), c.green(), c.blue());
}
void delay(int ms) {
    clock_t goal = ms + clock();
    while(goal > clock());
}

int main(int argc, char** argv) {
	init(argc, argv);
	SerialHandler serialHandler(PORT, BAUD_RATE);
	while(true) {
		updatePixels();
		int sumR = 0, sumG = 0, sumB = 0;
		const int x1 = 0, y1 = 0, x2 = SCR_W, y2 = SCR_H, SAMPLES = 5;
		for(int x = x1; x < x2; x += SAMPLES) {
			for(int y = y1; y < y2; y += SAMPLES) {
				Color color = getPixel(x, y);
				sumR += color.r; sumG += color.g; sumB += color.b;
			}
		}
		int totalPixels = ((x2 - x1) * (y2 - y1)) / (SAMPLES * SAMPLES);
		sumR /= totalPixels; sumG /= totalPixels; sumB /= totalPixels;
		int h = 0, s = 0, v = 0;
		RGBtoHSV(sumR, sumG, sumB, &h, &s, &v);
        v = (v * s) / 255;
		s = 255;
		HSVtoRGB(&sumR, &sumG, &sumB, h, s, v);
		serialHandler.writeSerial(sumR, sumG, sumB);
		delay(10);
	}
	deinit();
	return 0;
}
#endif
