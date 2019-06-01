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
		Vector3{-1, 0, 10},
		Vector3{0, 0, 0},
		Color{255, 0, 0}
	};

	VertexAttribute b = VertexAttribute{
		Vector3{1, 0, 10},
		Vector3{0, 0, 0},
		Color{0, 255, 0}
	};

	VertexAttribute c = VertexAttribute{
		Vector3{0, 1, 10},
		Vector3{0, 0, 0},
		Color{0, 0, 255}
	};

	VertexAttribute a1 = VertexAttribute{
		Vector3{-0.3, -0.3, 4},
		Vector3{0, 0, 0},
		Color{0, 0, 255}
	};

	VertexAttribute b1 = VertexAttribute{
		Vector3{0.3, -0.3, 10},
		Vector3{0, 0, 0},
		Color{0, 0, 255}
	};

	VertexAttribute c1 = VertexAttribute{
		Vector3{0, 0.3, 12},
		Vector3{0, 0, 0},
		Color{0, 0, 255}
	};

	float angle = 0.4f;

public:
	void OnUpdate() {
		clearScreen(Color{ 0, 0, 0 });
		angle += 0.1;

		a.Position = Vector3{ 2 * (float)cos(angle),	0, 10 + 2 * (float)sin(angle)};

		b.Position = Vector3{ 2 * (float)cos(angle + PI), 0, 10 + 2 * (float)sin(angle + PI)};

		c.Position = Vector3{ 0, 1, 10};

		Triangle t = Triangle{ a, b, c };
		Triangle t1 = Triangle{ a1, b1, c1 };
		Triangle buffer[2] = { t , t1};

		setTriangleBuffer(buffer, 2);
		drawTriangles();

		//drawClippedTri(t, 0);
		//drawClippedTri(t1, 0);
	}

	MainWindow(int w, int h) : BaseWindow(w, h){
		m_perspective = Matrix::CreatePerspectiveMatrix(75, 0.1f, 100);
		m_view = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}
};