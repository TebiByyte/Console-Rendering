#include "VectorMath.h"
#include <math.h>

#define PI 3.14159265358979323846

Vector3 Vector3::Normalize() { return *this / this->GetLength(); }

float Vector3::GetLength() { return sqrt(x * x + y * y + z * z); }

float Vector3::GetLengthSquared() { return x * x + y * y + z * z; }

Vector4 Vector3::ToVector4() { return { x, y, z, 1 }; }

float Vector3::Dot(Vector3& a, Vector3& b) { return a.x * b.x + a.y + b.y + a.z * b.z; }

Vector3 Vector3::Cross(Vector3& a, Vector3& b) {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x };
}

Vector3 operator+(Vector3& a, Vector3& b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
Vector3 operator-(Vector3& a, Vector3& b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
Vector3 operator/(Vector3& a, float b) { return { a.x / b, a.y / b, a.z / b }; }
Vector3 operator*(Vector3& a, float b) { return { a.x * b, a.y * b, a.z * b }; }
Vector3 operator*(float a, Vector3& b) { return { b.x * a, b.y * a, b.z * a }; }
Vector3 operator-(Vector3& a) { return { -a.x, -a.y, -a.z }; }
bool operator==(Vector3& a, Vector3& b) { return (a.x == b.x && a.y == b.y && a.z == b.z); }

Vector3 Vector4::ToVector3() { return { x / w, y / w, z / w }; }

