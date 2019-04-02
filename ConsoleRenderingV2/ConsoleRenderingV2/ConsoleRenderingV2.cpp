// ConsoleRenderingV2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "BaseWindow.h"
#include "Color.h"
#include "VectorMath.h"
#include <iostream>
#include "MainWindow.h"




int main()
{
	MainWindow win = MainWindow(400, 400);
	win.ConstructWindow(2, 2);
	win.BeginThread();

	/*BaseWindow MainWin = BaseWindow(256, 256);
	MainWin.ConstructWindow(2, 2);

	VertexAttribute a = VertexAttribute{
		Vector3{-0.3, -0.3, 2},
		Vector3{0, 0, 0},
		Color{255, 0, 0}
	};

	VertexAttribute b = VertexAttribute{
		Vector3{0.3, -0.3, 2},
		Vector3{0, 0, 0},
		Color{0, 255, 0}
	};

	VertexAttribute c = VertexAttribute{
		Vector3{0, 0.3, 2},
		Vector3{0, 0, 0},
		Color{0, 0, 255}
	};

	for (int x = 0; x < 256; x++) {
		for (int y = 0; y < 256; y++) {
			Color C = {x + y, y, 0 };

			MainWin.SetPixel(x, y, Color{ 0, 0, 0 });
		}
	}

	MainWin.DrawTriangle(a, b, c);




	MainWin.Render();

	MainWin.BeginThread();*/


}
