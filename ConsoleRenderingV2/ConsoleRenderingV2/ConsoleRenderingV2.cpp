// ConsoleRenderingV2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "BaseWindow.h"
#include "Color.h"

int main()
{
	BaseWindow MainWin = BaseWindow(256, 256);
	MainWin.ConstructWindow(3, 3);

	for (int x = 0; x < 256; x++) {
		for (int y = 0; y < 256; y++) {
			Color C = Color((x + y) / 2, 0, 0);
			MainWin.SetPixel(x, y, C);
		}
	}

	//MainWin.Render();

	MainWin.BeginThread();
}
