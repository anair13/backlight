#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include "HSV.h"
#include "include\boost\asio.hpp"
#include <Windows.h>
#include <SFML\Graphics.hpp>
#pragma comment(lib, "winmm.lib")

const int W = 200;
const int H = 200;

std::string PORT = "COM3";
const int BAUD_RATE = 9600;

void writeSerial(boost::asio::serial_port* serial, unsigned char r, unsigned char g, unsigned char b);

int SCR_W;
int SCR_H;

#if defined(_WIN32) || defined(_WIN64)
	boost::asio::io_service io = boost::asio::io_service();
	boost::asio::serial_port serial = boost::asio::serial_port(io, PORT);

	HWND hwnd;
	HDC hdc;
	HDC chdc;
	HBITMAP hbitmap;
	RGBQUAD* pixels;
	BITMAPINFO bmi;
	void init()
	{
		serial.set_option(boost::asio::serial_port_base::baud_rate(BAUD_RATE));
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
	void deinit()
	{
		delete[] pixels;

		ReleaseDC(hwnd, hdc);
		DeleteDC(chdc);
		DeleteObject(hbitmap);
	}
#endif

int main()
{
	init();
	
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(W, H), "BEARHack");

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
		}

		BitBlt(chdc, 0, 0, SCR_W, SCR_H, hdc, 0, 0, SRCCOPY | CAPTUREBLT);
		GetDIBits(chdc, hbitmap, 0, SCR_H, pixels, &bmi, DIB_RGB_COLORS);

		int sumR = 0, sumG = 0, sumB = 0;

		const int x1 = 0;
		const int y1 = 0;
		const int x2 = SCR_W;
		const int y2 = SCR_H;
		const int SAMPLES = 5;
		for(int x = x1; x < x2; x += SAMPLES)
		{
			for(int y = y1; y < y2; y += SAMPLES)
			{
				RGBQUAD c = pixels[x + y * SCR_W];
				
				BYTE r = c.rgbRed;
				BYTE g = c.rgbGreen;
				BYTE b = c.rgbBlue;

				sumR += r;
				sumG += g;
				sumB += b;
			}
		}
		int totalPixels = ((x2 - x1) * (y2 - y1)) / (SAMPLES * SAMPLES);
		sumR /= totalPixels;
		sumG /= totalPixels;
		sumB /= totalPixels;
		
		int h = 0, s = 0, v = 0;
		RGBtoHSV(sumR, sumG, sumB, &h, &s, &v);
		s = 255;
		HSVtoRGB(&sumR, &sumG, &sumB, h, s, v);

		writeSerial(&serial, sumR, sumG, sumB);

		window.clear();

		sf::RectangleShape rect(sf::Vector2f(W, H));
		rect.setFillColor(sf::Color(sumR, sumG, sumB));
		window.draw(rect);

		window.display();
		Sleep(10);
	}
	deinit();
}

void writeSerial(boost::asio::serial_port* serial, unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char c[4];
	c[0] = 0xFF;
	c[1] = r;
	c[2] = g;
	c[3] = b;
	boost::asio::write(*serial, boost::asio::buffer(c, sizeof(c)));
}

#endif