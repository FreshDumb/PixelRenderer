#include<cmath>

///	Basic Vector3 class with useful methods
/*
Defines a Vector3 and offers usefule methods like adding, subtracting, cross- and dotproduct
*/
#ifndef VEC_H
#define VEC_H
struct Vector3 {
public:
	float x, y, z;
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Vector3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

	static float dotProduct(const Vector3 &a, const Vector3 &b);
	static float dotProduct(float Ax, float Ay, float Az, float Bx, float By, float Bz);
	static Vector3 crossProduct(const Vector3 &a,const Vector3 &b);
	static float magnitude(const Vector3& a);
	static Vector3 normalize(const Vector3& a);
	static float distance(const Vector3 &a, const Vector3 &b);

	Vector3 operator+(const Vector3 &a) const;
	Vector3 operator*(const float a) const;
	Vector3 operator-();
	Vector3 operator-(const Vector3 &a) const;
};
#endif