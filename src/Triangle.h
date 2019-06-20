#include"Importer.h"

/// Class defining a Triangle
class Triangle {
public:
	Vector3 vertexA, vertexB, vertexC,
		EdgeAB, EdgeBC, EdgeCA , Normal;
	Triangle(Vector3 a, Vector3 b, Vector3 c);
	Triangle();
	static Triangle moveTriangle(Triangle triangle, Vector3 moveVector);
};
