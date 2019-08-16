#pragma once
#include "BaseWindow.h"
#include "VectorMath.h"
#include "Color.h"
#include "VertexAttribute.h"
#include <math.h>
#include <chrono>
#include <conio.h>
#include "Mesh.h"


#define PI 3.14159265358979323846

class MainWindow : public BaseWindow {
private:
	Vector3 cameraLocation = { 0, 0, -10 };
	Mesh test = Mesh("C:/Users/Redemption/Desktop/Monkey.obj");

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

		/*char key = _getch();//I may need to do this in a separate thread...

		if (key == 72) {
			cameraLocation = cameraLocation + Vector3{ 0, 0, 0.1f };
		}
		else if (key == 75) {
			cameraLocation = cameraLocation - Vector3{ 0.1f, 0, 0 };
		}
		else if (key == 77) {
			cameraLocation = cameraLocation + Vector3{ 0.1f, 0, 0 };
		}
		else if (key == 80) {
			cameraLocation = cameraLocation - Vector3{ 0, 0, 0.1f };
		}*/
		angle += 0.1f;

		setTriangleBuffer(test.m_data, test.triCount);
		drawTriangles();

		this->m_view = Matrix::CreateTranslationMatrix(cameraLocation);
		this->m_model = Matrix::CreateZRotationMatrix(angle);
	}

	MainWindow(int w, int h) : BaseWindow(w, h){
		m_perspective = Matrix::CreatePerspectiveMatrix(75, 0.1f, 100);

		m_view = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		m_model = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}
};