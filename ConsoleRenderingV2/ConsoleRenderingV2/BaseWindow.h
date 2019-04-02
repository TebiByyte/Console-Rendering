#pragma once
#include "Color.h"
#include "VertexAttribute.h"
#include <Windows.h>

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

public:
	int GetHeight() { return Height; }
	int GetWidth() { return Width; }
	int ConstructWindow(int, int);
	void BeginThread();
	void SetPixel(int, int, Color);
	void Render();
	void DrawTriangle(VertexAttribute&, VertexAttribute&, VertexAttribute&);
	void ClearScreen(Color);

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