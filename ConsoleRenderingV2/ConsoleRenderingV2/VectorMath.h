#pragma once
struct Vector3;
struct Vector4;
struct Matrix;

struct Vector3 {
	float x, y, z;

	float GetLength();
	float GetLengthSquared();
	Vector3 Normalize();
	Vector4 ToVector4();
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

struct Vector4 {
	float x, y, z, w;

	Vector3 ToVector3();
};

Vector4 operator+(const Vector4&, const Vector4&);
Vector4 operator-(const Vector4&, const Vector4&);
Vector4 operator/(const Vector4&, float);
Vector4 operator*(const Vector4&, float);
Vector4 operator*(float, const Vector4&);
Vector4 operator-(const Vector4&);
bool operator==(const Vector4&, const Vector4&);

struct Matrix {
	float Components[4][4];

	static Matrix Inverse(Matrix);
	static Matrix CreatePerspectiveMatrix(float, float, float);
};

void RowOperation(Matrix&, int, int, float);
void ScaleRow(Matrix&, int, float);

Matrix operator-(const Matrix&);
Matrix operator*(const Matrix&, float);
Matrix operator*(float, const Matrix&);
Vector4 operator*(const Matrix&, const Vector4&);
Matrix operator*(const Matrix&, const Matrix&);
Matrix operator/(const Matrix&, float);
