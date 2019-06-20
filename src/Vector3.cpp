#include "Vector3.h"

float Vector3::dotProduct(const Vector3 &a, const Vector3 &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


float Vector3::dotProduct(float Ax, float Ay, float Az, float Bx, float By, float Bz){
	return Ax * Bx + Ay * By + Az * Bz;
}
Vector3 Vector3::crossProduct(const Vector3& a,const Vector3& b) {
	return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
float Vector3::magnitude(const Vector3& a) {
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}
Vector3 Vector3::normalize(const Vector3& a) {
	float temp = magnitude(a);
	return Vector3(a.x / temp, a.y / temp, a.z / temp);
}
float Vector3::distance(const Vector3 &a, const Vector3 &b) {
	return abs(magnitude(b - a));
}


Vector3 Vector3::operator*(const float a) const {
	return Vector3(a * x, a * y, a * z);
}

Vector3 Vector3::operator+(const Vector3 &a) const {
	return Vector3(a.x + x, a.y + y, a.z + z);
}
Vector3 Vector3::operator-()
{
	return Vector3(-x, -y, -z);
}
Vector3 Vector3::operator-(const Vector3 &a) const {
	return Vector3(x - a.x,y - a.y,z- a.z);
}