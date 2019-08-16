#include "pch.h"
#include "BaseWindow.h"
#include "Color.h"
#include <Windows.h>
#include <math.h>
#include <iostream>
#include <chrono>
#include <string>
#include <list>
#include <iterator>

//TODO Fix triangle drawing so no gaps appear when trying to draw edge-connected triangles, refactor code. 
BOOL BaseWindow::m_isRunning = FALSE;

BaseWindow::BaseWindow(int Height = 128, int Width = 128) {
	this->m_height = Height;
	this->m_width =  Width;

	this->m_screenContents = new Color[Height * Width];
	this->m_screenBuffer = new CHAR_INFO[Height * Width];
	this->m_depthBuffer =    new float[Height * Width];
	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	m_rectWindow = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow);
}

int BaseWindow::constructWindow(int fontW, int fontH) {
	m_isRunning = TRUE;
	COORD coord = { (short)m_width, (short)m_height };
	if (!SetConsoleScreenBufferSize(m_hConsole, coord)) {
		throw "Failed to set screen buffer size.";
	}

	if (!SetConsoleActiveScreenBuffer(m_hConsole)) {
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
	if (!SetCurrentConsoleFontEx(m_hConsole, false, &cfi)) {
		throw "Failed to set the font.";
	}

	m_rectWindow = { 0, 0, (short)m_width - 1, (short)m_height - 1 };
	if (!SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow)) {
		throw "Failed to set console info";
	}

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)eventHandler, TRUE);

	return 1;
}

void BaseWindow::clearScreen(Color clearColor) {
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			setPixel(x, y, clearColor);
			setDepth(x, y, 1000);
		}
	}
}

void BaseWindow::beginThread() {
	while (BaseWindow::m_isRunning) {

		OnUpdate();
		render();
	}
}

inline int BaseWindow::getIndex(int x, int y) {
	return x + m_width * y;
}

void BaseWindow::setTriangleBuffer(Triangle t_tris[], int t_bufferLength) {
	m_triBuffer = t_tris;
	m_bufferLength = t_bufferLength;
}

VertexAttribute BaseWindow::intersectLinePlane(Vector3& n, Vector3& p, VertexAttribute& a, VertexAttribute& b) {
	n = n.Normalize();
	float dp = Vector3::Dot(n, p);
	float ad = Vector3::Dot(a.Position, n);
	float bd = Vector3::Dot(b.Position, n);
	float t = (dp - ad) / (bd - ad);
	
	return VertexAttribute{
		(b.Position - a.Position) * t + a.Position,
		Vector3{0, 0, 0}, //Just allow the normal to be zero for now.
		(b.VertexColor - a.VertexColor) * t + a.VertexColor
	};
}

int BaseWindow::clipTrianglePlane(Vector3 planePosition, Vector3 planeNormal, Triangle& inputTri, Triangle& outputTri1, Triangle& outputTri2) {
	planeNormal = planeNormal.Normalize();

	auto distFromPlane = [&](Vector3& p) {
		return Vector3::Dot(planeNormal, p - planePosition);
	};

	VertexAttribute* inside[3];
	int insideCount = 0;
	VertexAttribute* outside[3];
	int outsideCount = 0;

	float d0 = distFromPlane(inputTri.a.Position);
	float d1 = distFromPlane(inputTri.b.Position);
	float d2 = distFromPlane(inputTri.c.Position);

	if (d0 >= 0) { 
		inside[insideCount++] = &inputTri.a; 
	}
	else { outside[outsideCount++] = &inputTri.a; }

	if (d1 >= 0) { 
		inside[insideCount++] = &inputTri.b; 
	}
	else { outside[outsideCount++] = &inputTri.b; }

	if (d2 >= 0) 
	{ 
		inside[insideCount++] = &inputTri.c; 
	}
	else { outside[outsideCount++] = &inputTri.c; }

	if (insideCount == 0) {
		return 0;
	}
	if (insideCount == 3) {
		outputTri1 = inputTri;

		return 1;
	}
	if (insideCount == 1 && outsideCount == 2) {

		outputTri1.a = *inside[0];
		outputTri1.b = intersectLinePlane(planeNormal, planePosition, *inside[0], *outside[0]);
		outputTri1.c = intersectLinePlane(planeNormal, planePosition, *inside[0], *outside[1]);

		return 1;
	}
	if (insideCount == 2 && outsideCount == 1) {
		outputTri1.a = *inside[0];
		outputTri1.b = *inside[1];
		outputTri1.c = intersectLinePlane(planeNormal, planePosition, *inside[0], *outside[0]);

		outputTri2.a = *inside[1];
		outputTri2.b = outputTri1.c;
		outputTri2.c = intersectLinePlane(planeNormal, planePosition, *inside[1], *outside[0]);

		return 2;
	}
}

void BaseWindow::drawTriangle(VertexAttribute& a, VertexAttribute& b, VertexAttribute& c) {
	Vector3 screenCoordA = Vector3{ floor(m_width * a.Position.x + m_width / 2), floor(m_height * a.Position.y + m_height / 2), 0 };
	Vector3 screenCoordB = Vector3{ floor(m_width * b.Position.x + m_width / 2), floor(m_height * b.Position.y + m_height / 2), 0 };
	Vector3 screenCoordC = Vector3{ floor(m_width * c.Position.x + m_width / 2), floor(m_height * c.Position.y + m_height / 2), 0 };

	Vector3 UnsortedA = screenCoordA;
	Vector3 UnsortedB = screenCoordB;
	Vector3 UnsortedC = screenCoordC;

	//We need to sort the screencoords by y value
	//A should be the top most, b needs to be in the middle, and c needs to be last. 

	auto SwapPositions = [](Vector3& a, Vector3& b) {
		float tempx = b.x;
		float tempy = b.y;
		b.x = a.x;
		b.y = a.y;
		a.x = tempx;
		a.y = tempy;
	};

	if (screenCoordA.y > screenCoordB.y)
		SwapPositions(screenCoordA, screenCoordB);
	if (screenCoordB.y > screenCoordC.y)
		SwapPositions(screenCoordB, screenCoordC);
	if (screenCoordA.y > screenCoordB.y)
		SwapPositions(screenCoordA, screenCoordB);

	auto fillLine = [this, a, b, c, UnsortedA, UnsortedB, UnsortedC](int x1, int x2, int y) {
		Vector3 ab = UnsortedB - UnsortedA;
		Vector3 ac = UnsortedC - UnsortedA;
		Vector3 bc = UnsortedC - UnsortedB;

		float Z1 = 1 / a.Position.z;
		float Z2 = 1 / b.Position.z;
		float Z3 = 1 / c.Position.z;

		if (x2 < x1) {
			int temp = x2;
			x2 = x1;
			x1 = temp;
		}

		for (int x = x1; x < x2; x++) {
			Vector3 q = Vector3{ UnsortedA.x - x, UnsortedA.y - y, 0 };

			float s = abs(Vector3::Cross(q, ac).z) / Vector3::Cross(ab, ac).GetLength();
			float t = abs(Vector3::Cross(ab, q).z) / Vector3::Cross(ab, ac).GetLength();
			float u = 1 - (s + t);

			float depth = Z1 * u + Z2 * s + Z3 * t;
			
			if (1 / depth <= getDepth(x, y)) {
				Color color = (a.VertexColor * u * Z1 + b.VertexColor * s * Z2 + c.VertexColor * t * Z3) * (1 / depth);

				//This will eventually invoke a shader program with the interpolated position, normal, and vertex color. 
				setDepth(x, y, 1 / depth);
				setPixel(x, y, color);
			}
		}
	};

	auto fillBottomFlatTri = [fillLine](Vector3 a, Vector3 b, Vector3 c) {
		float invslope1 = (b.x - a.x) / (b.y - a.y);
		float invslope2 = (c.x - a.x) / (c.y - a.y);

		float curx1 = a.x;
		float curx2 = a.x;

		for (int y = round(a.y); y <= b.y; y++) {
			fillLine(curx1, curx2, y);
			curx1 += invslope1;
			curx2 += invslope2;
		}
	};

	auto fillTopFlatTri = [fillLine](Vector3 a, Vector3 b, Vector3 c) {
		float invslope1 = (c.x - a.x) / (c.y - a.y);
		float invslope2 = (c.x - b.x) / (c.y - b.y);

		float curx1 = c.x;
		float curx2 = c.x;

		for (int y = round(c.y); y > a.y; y--) {
			fillLine(curx1, curx2, y);
			curx1 -= invslope1;
			curx2 -= invslope2;
		}
	};

	if (abs(screenCoordB.y - screenCoordC.y) < 1) {
		fillBottomFlatTri(screenCoordA, screenCoordB, screenCoordC);
	}
	else if (abs(screenCoordA.y - screenCoordC.y) < 1) {
		fillTopFlatTri(screenCoordA, screenCoordB, screenCoordC);
	}
	else {
		Vector3 screenCoordD = Vector3{ screenCoordA.x + ((screenCoordB.y - screenCoordA.y) / (screenCoordC.y - screenCoordA.y))*(screenCoordC.x - screenCoordA.x), screenCoordB.y, 0 };

		fillBottomFlatTri(screenCoordA, screenCoordB, screenCoordD);
		fillTopFlatTri(screenCoordB, screenCoordD, screenCoordC);
	}
}

void BaseWindow::drawClippedTri(Triangle& tri, int clipAgainst) {
	//0 clips against the near plane
	//1 clips against the far plane
	//2 clips against the top plane
	//3 clips against the bottom plane
	//4 clips against the right plane
	//5 clips against the left plane

	Triangle out1;
	Triangle out2;
	Vector3 planeNormal = {};
	Vector3 planePosition = {};

	switch (clipAgainst) {
		case(0):{
			planeNormal = { 0, 0, 1 };
			planePosition = { 0, 0, 0.1f };//I'll add a constant for the near plane later.
			break;
		}
		case(1): {
			planeNormal = { 0, 0, -1 };
			planePosition = { 0, 0, 50 };
			break;
		}
		case(2): {
			planeNormal = { 0, -1, 0 };
			planePosition = { 0, 0.5f, 0 };
			break;
		}
		case(3): {
			planeNormal = { 0, 1, 0 };
			planePosition = { 0, -0.5f, 0 };
			break;
		}
		case(4): {
			planeNormal = { -1, 0, 0 };
			planePosition = { 0.5f, 0, 0 };
			break;
		}
		case(5): {
			planeNormal = { 1, 0, 0 };
			planePosition = { -0.5f, 0, 0 };
			break;
		}
	}

	int numOut = clipTrianglePlane(planePosition, planeNormal, tri, out1, out2);

	if (numOut == 0) { 

		return; 
	}
	else if (numOut == 1) {
		if (clipAgainst < 5) {
			drawClippedTri(out1, clipAgainst + 1);
		}
		else {
			drawTriangle(out1.a, out1.b, out1.c);
		}
	}
	else if (numOut == 2) { 
		if (clipAgainst < 5) {
			drawClippedTri(out1, clipAgainst + 1);
			drawClippedTri(out2, clipAgainst + 1);
		}
		else {
			drawTriangle(out1.a, out1.b, out1.c);
			drawTriangle(out2.a, out2.b, out2.c);
		}
	}
}

void BaseWindow::drawTriangles() {
	for (int i = 0; i < m_bufferLength; i++) {
		Vector4 aPosition = m_perspective * Matrix::Inverse(m_view) * m_model * m_triBuffer[i].a.Position.ToVector4();
		Vector4 bPosition = m_perspective * Matrix::Inverse(m_view) * m_model * m_triBuffer[i].b.Position.ToVector4();
		Vector4 cPosition = m_perspective * Matrix::Inverse(m_view) * m_model * m_triBuffer[i].c.Position.ToVector4();

		Triangle view = {
			{aPosition.ToVector3(), {0, 0, 0}, m_triBuffer[i].a.VertexColor},
			{bPosition.ToVector3(), {0, 0, 0}, m_triBuffer[i].b.VertexColor},
			{cPosition.ToVector3(), {0, 0, 0}, m_triBuffer[i].c.VertexColor}
		};

		drawClippedTri(view, 0);
	}
}

void BaseWindow::render() {
	//First, dither the color buffer

	float e1 = (7.0f / 16.0f);
	float e2 = (3.0f / 16.0f);
	float e3 = (5.0f / 16.0f);
	float e4 = (1.0f / 16.0f);

	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			Color currentPixel = m_screenContents[getIndex(x, y)];

			short R = currentPixel.R < 85 ? 0 : 170;
			short G = currentPixel.G < 85 ? 0 : 170;
			short B = currentPixel.B < 85 ? 0 : 170;
			float L = currentPixel.Luminance();

			Color newPixel = Color{ R, G, B };

			newPixel = newPixel * (L < 128 ? 1 : 2);

			Color error = currentPixel - newPixel;

			m_screenContents[getIndex(x, y)] = newPixel;
			
			/*if (x < m_width - 1)
				setPixel(x + 1, y,     (error * e1) + m_screenContents[getIndex(x + 1, y    )]);
			if (x > 0 && y < m_height - 1)
				setPixel(x - 1, y + 1, (error * e2) + m_screenContents[getIndex(x - 1, y + 1)]);
			if (y < m_height - 1)
				setPixel(x,     y + 1, (error * e3) + m_screenContents[getIndex(x,     y + 1)]);
			//if (x < Width - 1 && y < Height - 1)*/
				//SetPixel(x + 1, y + 1, (error * e4) + ScreenContents[getIndex(x + 1, y + 1)]);
			//I have no clue why the dithering doesn't work properly with this last line. 

			short r = newPixel.R < 128 ? 0 : 0b01000000;
			short g = newPixel.G < 128 ? 0 : 0b00100000;
			short b = newPixel.B < 128 ? 0 : 0b00010000;
			short l = L < 128 ? 0 : 0b10000000;
			
			m_screenBuffer[getIndex(x, y)].Attributes = l | r | g | b;
			m_screenBuffer[getIndex(x, y)].Char.AsciiChar = ' ';
		}
	}

	WriteConsoleOutput(m_hConsole, m_screenBuffer, { (short)m_width, (short)m_height }, { 0, 0 }, &m_rectWindow);
}

float BaseWindow::getDepth(int x, int y) {
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		return m_depthBuffer[x + m_width * y];
	else
		return 0;
}

void BaseWindow::setDepth(int x, int y, float d) {
	if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
		m_depthBuffer[x + m_width * y] = d;
	}
}

inline void BaseWindow::setPixel(int x, int y, Color c) {
	if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
		m_screenContents[x + m_width * y] = c;
	}
}
