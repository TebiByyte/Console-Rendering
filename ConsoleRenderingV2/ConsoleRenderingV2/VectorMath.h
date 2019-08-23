#pragma once
struct Vector3;
struct Matrix;

struct Vector3 {
	float x = 0, y = 0, z = 0, w = 1;

	float GetLength();
	float GetLengthSquared();
	Vector3 Normalize();
	static float Dot(const Vector3&, const Vector3&);
	static Vector3 Cross(const Vector3&, const Vector3&);
};

Vector3 operator+(const Vector3&, const Vector3&);
Vector3 operator-(const Vector3&, const Vector3&);
Vector3 operator/(const Vector3&, float);
Vector3 operator*(const Vector3&, float);
Vector3 operator*(float, const Vector3&);
Vector3 operator-(const Vector3&);
bool operator==(const Vector3&, const Vector3&);

struct Matrix {
	float Components[4][4];

	static Matrix Inverse(Matrix);
	static Matrix Transpose(Matrix);
	static Matrix CreatePerspectiveMatrix(float, float, float);
	static Matrix CreateTranslationMatrix(Vector3);
	static Matrix CreateZRotationMatrix(float);
};

void RowOperation(Matrix&, int, int, float);
void ScaleRow(Matrix&, int, float);

Matrix operator-(const Matrix&);
Matrix operator*(const Matrix&, float);
Matrix operator*(float, const Matrix&);
Vector3 operator*(const Matrix&, const Vector3&);
Matrix operator*(const Matrix&, const Matrix&);
Matrix operator/(const Matrix&, float);
