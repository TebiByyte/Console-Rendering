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
		Color{120, 120, 120}
	};

	VertexAttribute b = VertexAttribute{
		Vector3{0.3, -0.3, 2},
		Vector3{0, 0, 0},
		Color{120, 120, 120}
	};

	VertexAttribute c = VertexAttribute{
		Vector3{0, 0.3, 2},
		Vector3{0, 0, 0},
		Color{120, 120, 120}
	};

	float angle = 0.4f;

public:
	void OnUpdate() {
		ClearScreen(Color{ 0, 0, 0 });
		angle += 0.1;

		a.Position = Vector3{ 0.9f * (float)cos(angle),					0.9f * (float)sin(angle),				  1 };
		b.Position = Vector3{ 0.9f * (float)cos(angle + PI * 2 / 3.0f), 0.9f * (float)sin(angle + PI * 2 / 3.0f), 1 };
		c.Position = Vector3{ 0.9f * (float)cos(angle + PI * 4 / 3.0f), 0.9f * (float)sin(angle + PI * 4 / 3.0f), 1 };

		//DrawTriangle(a, c, b);
		Triangle t = Triangle{ a, b, c };
		DrawClippedTri(t, 0);
	}

	MainWindow(int w, int h) : BaseWindow(w, h){

	}
};