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
#include <gtkmm.h>
#include <gdkmm.h>
#include <time.h>

std::string PORT = "/dev/ttyACM0";

Glib::RefPtr<Gtk::Application> app;

Gtk::Main kit;
Glib::RefPtr<Gdk::Window> win;
Glib::RefPtr<Gdk::Pixbuf> pb;
BYTE* pixels;

int rowstride;
int channels;

void init() {
    Glib::RefPtr<Gdk::Screen> screen = Gdk::Screen::get_default();
    SCR_W = screen->get_width();
    SCR_H = screen->get_height();
    win = screen->get_root_window();
    pb = Gdk::Pixbuf::create(win, 0, 0, SCR_W, SCR_H);
    pixels = pb->get_pixels();
    rowstride = pb->get_rowstride();
    channels = pb->get_n_channels();
}
void deinit() {
}
void updatePixels() {
    pb = Gdk::Pixbuf::create(win, 0, 0, SCR_W, SCR_H);
    pixels = pb->get_pixels();
}
Color getPixel(int x, int y) {
    BYTE r = pixels[y * rowstride + x * channels];
    BYTE g = pixels[y * rowstride + x * channels + 1]; 
    BYTE b = pixels[y * rowstride + x * channels + 2];
    return Color(r, g, b);
}
void delay(int ms) {
    clock_t goal = ms + clock();
    while(goal > clock());
}

int main() {
	init();
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
