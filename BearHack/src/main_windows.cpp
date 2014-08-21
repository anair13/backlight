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

#include "SerialHandlerWindows.h"
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

std::string PORT = "COM3";

HWND hwnd;
HDC hdc;
HDC chdc;
HBITMAP hbitmap;
RGBQUAD* pixels;
BITMAPINFO bmi;

void init() {
	SCR_W = GetSystemMetrics(SM_CXSCREEN);
	SCR_H = GetSystemMetrics(SM_CYSCREEN);
	
	hwnd = GetDesktopWindow();
	hdc = GetDC(hwnd);
	chdc = CreateCompatibleDC(hdc);
	hbitmap = CreateCompatibleBitmap(hdc, SCR_W, SCR_H);
	SelectObject(chdc, hbitmap);
	
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = SCR_W;
	bmi.bmiHeader.biHeight = SCR_H;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	pixels = new RGBQUAD[SCR_W * SCR_H];
}
void deinit() {
	delete[] pixels;

	ReleaseDC(hwnd, hdc);
	DeleteDC(chdc);
	DeleteObject(hbitmap);
}
void updatePixels() {
	BitBlt(chdc, 0, 0, SCR_W, SCR_H, hdc, 0, 0, SRCCOPY | CAPTUREBLT);
	GetDIBits(chdc, hbitmap, 0, SCR_H, pixels, &bmi, DIB_RGB_COLORS);
}
Color getPixel(int x, int y) {
	RGBQUAD c = pixels[x + y * SCR_W];
	return Color(c.rgbRed, c.rgbGreen, c.rgbBlue);
}
void delay(int ms) {
	Sleep(ms);
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
