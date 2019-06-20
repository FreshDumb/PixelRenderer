#include"Triangle.h"

/// Constructor for triangle
//	calculates the edges and normal and stores them in the object instance
//	normals aren't really used currently
Triangle::Triangle(Vector3 a, Vector3 b, Vector3 c) {
	vertexA = a;
	vertexB = b;
	vertexC = c;
	EdgeAB = b - a;
	EdgeBC = c - b;
	EdgeCA = a - c;
	Normal = Vector3::normalize(Vector3::crossProduct(EdgeAB, EdgeBC));
}
Triangle::Triangle() {}

/// Method to move a triangle in moveVector direction
//	not used in project
Triangle Triangle::moveTriangle(Triangle triangle, Vector3 moveVector) {
	Vector3 newA, newB, newC;
	newA = triangle.vertexA + moveVector;
	newB = triangle.vertexB + moveVector;
	newC = triangle.vertexC + moveVector;
	return Triangle(newA, newB, newC);
}