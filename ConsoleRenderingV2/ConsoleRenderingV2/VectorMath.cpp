#include "pch.h"
#include "VectorMath.h"
#include <math.h>

#define PI 3.14159265358979323846

//Define the methods and operator overloads for the Vector3 struct
Vector3 Vector3::Normalize() { return *this / this->GetLength(); }

float Vector3::GetLength() { return sqrt(x * x + y * y + z * z); }

float Vector3::GetLengthSquared() { return x * x + y * y + z * z; }

Vector4 Vector3::ToVector4() { return { x, y, z, 1 }; }

float Vector3::Dot(const Vector3& a, const Vector3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b) {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x };
}

Vector3 operator+(const Vector3& a, const Vector3& b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
Vector3 operator-(const Vector3& a, const Vector3& b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
Vector3 operator/(const Vector3& a, float b) { return { a.x / b, a.y / b, a.z / b }; }
Vector3 operator*(const Vector3& a, float b) { return { a.x * b, a.y * b, a.z * b }; }
Vector3 operator*(float a, const Vector3& b) { return { b.x * a, b.y * a, b.z * a }; }
Vector3 operator-(const Vector3& a) { return { -a.x, -a.y, -a.z }; }
bool operator==(const Vector3& a, const Vector3& b) { return (a.x == b.x && a.y == b.y && a.z == b.z); }
//Define the methods and operator overloads for the Vector4 struct

Vector3 Vector4::ToVector3() { return { x / w, y / w, z / w }; }

Vector4 operator+(const Vector4& a, const Vector4& b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }
Vector4 operator-(const Vector4& a, const Vector4& b) { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }
Vector4 operator/(const Vector4& a, const float b) { return { a.x / b, a.y / b, a.z / b, a.w / b }; }
Vector4 operator*(const Vector4& a, const float b) { return { a.x * b, a.y * b, a.z * b, a.w * b }; }
Vector4 operator*(const float b, const Vector4& a) { return { a.x * b, a.y * b, a.z * b, a.w * b }; }
Vector4 operator-(const Vector4& a) { return { -a.x, -a.y, -a.z, -a.w }; }
bool operator==(const Vector4& a, const Vector4& b) { return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w); }

//Define the methods and operator overloads for the Matrix struct

Matrix operator-(const Matrix& a) {
	Matrix result = {};

	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			result.Components[r][c] = -a.Components[r][c];
		}
	}

	return result;
}

Matrix operator*(const Matrix& a, float b) {
	Matrix result = {};

	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			result.Components[r][c] *= b;
		}
	}

	return result;
}

Matrix operator*(float b, const Matrix& a) {
	Matrix result = {};

	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			result.Components[r][c] *= b;
		}
	}

	return result;
}

Vector4 operator*(const Matrix& a, const Vector4& b) {
	return {
		a.Components[0][0] * b.x + a.Components[0][1] * b.y + a.Components[0][2] * b.z + a.Components[0][3] * b.w,
		a.Components[1][0] * b.x + a.Components[1][1] * b.y + a.Components[1][2] * b.z + a.Components[1][3] * b.w,
		a.Components[2][0] * b.x + a.Components[2][1] * b.y + a.Components[2][2] * b.z + a.Components[2][3] * b.w,
		a.Components[3][0] * b.x + a.Components[3][1] * b.y + a.Components[3][2] * b.z + a.Components[3][3] * b.w
	};
}

Matrix operator*(const Matrix& a, const Matrix& b) {
	Matrix result = {};

	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			result.Components[r][c] = a.Components[r][0] * b.Components[0][c] + a.Components[r][1] * b.Components[1][c] + a.Components[r][2] * b.Components[2][c] + a.Components[r][3] * b.Components[3][c];
		}
	}

	return result;
}

Matrix operator/(const Matrix& a, float b) {
	Matrix result = {};

	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			result.Components[r][c] /= b;
		}
	}

	return result;
}

void RowOperation(Matrix& a, int r0, int r1, float m) {
	for (int c = 0; c < 4; c++) {
		a.Components[r0][c] += m * a.Components[r1][c];
	}
}

void ScaleRow(Matrix& a, int r0, float m) {
	for (int c = 0; c < 4; c++) {
		a.Components[r0][c] /= m;
	}
}

Matrix Matrix::Inverse(Matrix a) {
	Matrix result = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	for (int i = 0; i < 4; i++) {
		ScaleRow(result , i, a.Components[i][i]);
		ScaleRow(a, i, a.Components[i][i]);

		for (int j = 0; j < 4; j++) {
			if (j != i) {
				RowOperation(result, j, i, -a.Components[j][i]);
				RowOperation(a, j, i, -a.Components[j][i]);
			}
		}
	}

	return result;
}

//TODO: Include aspect ratio in this function.
Matrix Matrix::CreatePerspectiveMatrix(float fov, float near, float far) {
	float S = 1 / (tan(fov / 3 * PI / 180));
	float f = -far / (far - near);

	return {
		S, 0, 0, 0,
		0, S, 0, 0,
		0, 0, -(far + near) / (far - near), -2 * f * near,
		0, 0, -1, 0
	};
}

Matrix Matrix::CreateTranslationMatrix(Vector3 t_position) {
	return {
		1, 0, 0, t_position.x,
		0, 1, 0, t_position.y,
		0, 0, 1, t_position.z,
		0, 0, 0, 1
	};
}

Matrix Matrix::CreateZRotationMatrix(float angle) {
	float cosA = (float)cos(angle);
	float sinA = (float)sin(angle);

	return {
		cosA,  0, sinA, 0,
		0,     1, 0,    0,
		-sinA, 0, cosA, 0,
		0,     0, 0,    1
	};
}