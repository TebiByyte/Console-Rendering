#pragma once
#include "Color.h"
#include "VertexAttribute.h"
#include "Triangle.h"
#include <Windows.h>
#include <list>

//TODO: Add some virtual methods for things to do on updating

class BaseWindow {
private:
	SMALL_RECT RectWindow;
	CHAR_INFO* ScreenBuffer;
	HANDLE hConsole;
	int Height;
	int Width;
	Color* ScreenContents;
	float* DepthBuffer;
	void setDepth(int, int, float);
	float getDepth(int, int);
	static BOOL isRunning;
	std::list<Triangle> triQueue;
	VertexAttribute intersectLinePlane(Vector3&, Vector3&, VertexAttribute&, VertexAttribute&);
	int clipTrianglePlane(Vector3, Vector3, Triangle&, Triangle&, Triangle&);

public:
	int GetHeight() { return Height; }
	int GetWidth() { return Width; }
	int ConstructWindow(int, int);
	void BeginThread();
	void SetPixel(int, int, Color);
	void Render();
	void DrawTriangle(VertexAttribute&, VertexAttribute&, VertexAttribute&);
	void ClearScreen(Color);
	void DrawTriangles();
	void PushTriangle(Triangle);
	void DrawClippedTri(Triangle&, int);

	BaseWindow(int, int);

protected:
	static BOOL EventHandler(DWORD evt) {
		if (evt == CTRL_CLOSE_EVENT) {
			BaseWindow::isRunning = FALSE;
		}

		return TRUE;
	}

	virtual void OnUpdate() {}

	int getIndex(int, int);
};