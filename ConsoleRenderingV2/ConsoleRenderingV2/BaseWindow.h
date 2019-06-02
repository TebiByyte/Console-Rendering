#pragma once
#include "Color.h"
#include "VertexAttribute.h"
#include "Triangle.h"
#include <Windows.h>
#include <list>

//TODO: Add some virtual methods for things to do on updating

class BaseWindow {
private:
	SMALL_RECT m_rectWindow;
	CHAR_INFO* m_screenBuffer;
	HANDLE m_hConsole;
	Color* m_screenContents;
	float* m_depthBuffer;

	VertexAttribute intersectLinePlane(Vector3&, Vector3&, VertexAttribute&, VertexAttribute&);
	int clipTrianglePlane(Vector3, Vector3, Triangle&, Triangle&, Triangle&);

protected:
	static BOOL m_isRunning;
	Matrix m_perspective;
	Matrix m_view;
	Triangle* m_triBuffer;
	int m_bufferLength;
	int m_height;
	int m_width;

	void setDepth(int, int, float);
	float getDepth(int, int);
	//LRESULT CALLBACK WindowProc()

public:
	int getHeight() { return m_height; }
	int getWidth() { return m_width; }
	int constructWindow(int, int);
	void beginThread();
	void setPixel(int, int, Color);
	void render();
	void drawTriangle(VertexAttribute&, VertexAttribute&, VertexAttribute&);
	void clearScreen(Color);
	void drawTriangles();
	void drawClippedTri(Triangle&, int);
	void setTriangleBuffer(Triangle[], int);

	BaseWindow(int, int);

protected:
	static BOOL eventHandler(DWORD evt) {
		if (evt == CTRL_CLOSE_EVENT) {
			BaseWindow::m_isRunning = FALSE;
		}

		return TRUE;
	}

	virtual void OnUpdate() {}

	int getIndex(int, int);
};