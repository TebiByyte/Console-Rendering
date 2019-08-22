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
	Vector3 cameraLocation = { 0, 0, 5 };
	Mesh test = Mesh("C:/Users/Redemption/Desktop/Monkey.obj");

	float angle = 0.4f;

public:
	void OnUpdate() {
		clearScreen(Color{ 0, 0, 0 });
		this->m_view = Matrix::CreateTranslationMatrix(cameraLocation);
		this->m_model = Matrix::CreateZRotationMatrix(angle);

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
	}

	MainWindow(int w, int h) : BaseWindow(w, h){
		m_perspective = Matrix::CreatePerspectiveMatrix(75.0f, 0.1f, 100.0f);

		m_view = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		//m_perspective = m_view;

		m_model = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}
};