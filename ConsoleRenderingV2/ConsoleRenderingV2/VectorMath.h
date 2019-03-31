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
	static float Dot(Vector3&, Vector3&);
	static Vector3 Cross(Vector3&, Vector3&);
};

Vector3 operator+(Vector3&, Vector3&);
Vector3 operator-(Vector3&, Vector3&);
Vector3 operator/(Vector3&, float);
Vector3 operator*(Vector3&, float);
Vector3 operator*(float, Vector3&);
Vector3 operator-(Vector3&);
bool operator==(Vector3&, Vector3&);

struct Vector4 {
	float x, y, z, w;

	Vector3 ToVector3();
};

/*Vector4 operator+(Vector4&, Vector4&);
Vector4 operator-(Vector4&, Vector4&);
Vector4 operator/(Vector4&, float);
Vector4 operator*(Vector4&, float);
Vector4 operator*(float, Vector4&);
bool operator==(Vector4&, Vector4&);

struct Matrix {
	float Components[4][4];

	Matrix(float[4][4]);
	static Matrix Inverse(Matrix&);
	static Matrix CreatePerspectiveMatrix(float, float, float);
};

void RowOperation(Matrix&, int, int, float);
void ScaleRow(Matrix&, int, float);

Matrix operator-(Matrix&);
Matrix operator*(Matrix&, float);
Matrix operator*(float, Matrix&);
Matrix operator*(Matrix&, Vector4&);
Matrix operator*(Matrix&, Matrix&);
Matrix operator/(Matrix&, float);*/
