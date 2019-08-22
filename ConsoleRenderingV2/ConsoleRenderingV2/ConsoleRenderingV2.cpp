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
	win.constructWindow(2, 2);
	win.beginThread();


}
