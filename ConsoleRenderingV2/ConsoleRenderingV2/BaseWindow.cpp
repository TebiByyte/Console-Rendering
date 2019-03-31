#include "pch.h"
#include "BaseWindow.h"
#include "Color.h"
#include <Windows.h>
#include <math.h>
#include <iostream>

BOOL BaseWindow::isRunning = FALSE;

BaseWindow::BaseWindow(int Height = 128, int Width = 128) {
	this->Height = Height;
	this->Width =  Width;

	this->ScreenContents = new Color[Height * Width];
	this->ScreenBuffer = new CHAR_INFO[Height * Width];
	this->DepthBuffer =    new float[Height * Width];
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	RectWindow = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(hConsole, TRUE, &RectWindow);
}

int BaseWindow::ConstructWindow(int fontW, int fontH) {
	isRunning = TRUE;
	COORD coord = { (short)Width, (short)Height };
	if (!SetConsoleScreenBufferSize(hConsole, coord)) {
		throw "Failed to set screen buffer size.";
	}

	if (!SetConsoleActiveScreenBuffer(hConsole)) {
		throw "Failed to set screen buffer.";
	}

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontW;
	cfi.dwFontSize.Y = fontH;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	wcscpy_s(cfi.FaceName, L"Consolas");
	if (!SetCurrentConsoleFontEx(hConsole, false, &cfi)) {
		throw "Failed to set the font.";
	}

	RectWindow = { 0, 0, (short)Width - 1, (short)Height - 1 };
	if (!SetConsoleWindowInfo(hConsole, TRUE, &RectWindow)) {
		throw "Failed to set console info";
	}

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)EventHandler, TRUE);

	return 1;
}

void BaseWindow::BeginThread() {
	//Idk how to check if the console window is closed, so just keep going through it. 
	while (BaseWindow::isRunning) {
		Render();
	}
}

int BaseWindow::getIndex(int x, int y) {
	return x + Width * y;
}

void BaseWindow::Render() {
	//First, dither the color buffer

	float e1 = (7.0f / 16.0f);
	float e2 = (3.0f / 16.0f);
	float e3 = (5.0f / 16.0f);
	float e4 = (1.0f / 16.0f);

	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			Color currentPixel = ScreenContents[getIndex(x, y)];

			short R = currentPixel.R < 128 ? 0 : 255;
			short G = currentPixel.G < 128 ? 0 : 255;
			short B = currentPixel.B < 128 ? 0 : 255;

			Color newPixel = Color(R, G, B);
			Color error = currentPixel - newPixel;

			ScreenContents[getIndex(x, y)] = newPixel;

			if (x < Width - 1)
				ScreenContents[getIndex(x + 1, y)] = ScreenContents[getIndex(x + 1, y    )] + (error * e1);
			if (x > 0 && y < Height - 1)
				ScreenContents[getIndex(x - 1, y + 1)] = ScreenContents[getIndex(x - 1, y + 1)] + (error * e2);
			if (y < Height - 1)
				ScreenContents[getIndex(x, y + 1)] = ScreenContents[getIndex(x,     y + 1)] + (error * e3);
			if (x < Width - 1 && y < Height - 1)
				ScreenContents[getIndex(x + 1, y + 1)] = ScreenContents[getIndex(x + 1, y + 1)] + (error * e4);

			short r = newPixel.R < 128 ? 0 : 0b000001100;
			short g = newPixel.G < 128 ? 0 : 0b000001010;
			short b = newPixel.B < 128 ? 0 : 0b000001001;
			

			ScreenBuffer[getIndex(x, y)].Attributes = r | g | b;
			ScreenBuffer[getIndex(x, y)].Char.AsciiChar = '#';
		}
	}

	WriteConsoleOutput(hConsole, ScreenBuffer, { (short)Width, (short)Height }, { 0, 0 }, &RectWindow);
}

float BaseWindow::getDepth(int x, int y) {
	return 0;
}

void BaseWindow::setDepth(int x, int y, float d) {

}

void BaseWindow::SetPixel(int x, int y, Color c) {
	if (x < Width && x >= 0 && y < Height && y >= 0) {
		ScreenContents[x + Width * y] = c;
	}
}