#pragma once
#include "BaseWindow.h"
#include "VectorMath.h"
#include "Color.h"
#include "VertexAttribute.h"
#include <math.h>
#include <chrono>

#define PI 3.14159265358979323846

class MainWindow : public BaseWindow {
private:
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

	float angle = 0;

public:
	void OnUpdate() {
		ClearScreen(Color{ 0, 0, 0 });
		angle += 0.1;

		a.Position = Vector3{ 0.3f * (float)cos(angle), 0.3f * (float)sin(angle), 2 };
		b.Position = Vector3{ 0.3f * (float)cos(angle + PI * 2 / 3.0f), 0.3f * (float)sin(angle + PI * 2 / 3.0f), 2 };
		c.Position = Vector3{ 0.3f * (float)cos(angle + PI * 4 / 3.0f), 0.3f * (float)sin(angle + PI * 4 / 3.0f), 2 };

		DrawTriangle(a, c, b);
	}

	MainWindow(int w, int h) : BaseWindow(w, h){

	}
};