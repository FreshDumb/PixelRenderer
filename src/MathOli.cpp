#include"MathOli.h"
#include<limits>

RayHit::RayHit() : t(0), u(0), v(0) {
	triangle = Triangle();
}

const float MathOli::kEpsilon = 1e-8f;


/// Takes a given Triangle, an origin and a direction to see if the given Triangle was hit
//	Returns true on successful hit :)
bool MathOli::calcHit(Vector3 origin, Vector3 dir, Triangle triangle, RayHit&_Hit,Vector3 normals[3], Vector3 text[3])
{
	RayHit tempHit = RayHit();
	_Hit = tempHit;

	float t, u, v;

	Vector3 pVector = Vector3::crossProduct(dir, -triangle.EdgeCA);
	float determinante = Vector3::dotProduct(triangle.EdgeAB, pVector);


	/// With backface culling
	//if (determinante < kEpsilon) {
	//	return false;
	//}

	/// Without backface culling
	if (abs(determinante) < kEpsilon)
		return false;

	float inverseDeterminate = 1 / determinante;
	Vector3 tVector = origin - triangle.vertexA;

	u = Vector3::dotProduct(tVector, pVector) * inverseDeterminate;
	if (u < 0 || u > 1) return false;

	Vector3 qVector = Vector3::crossProduct(tVector, triangle.EdgeAB);
	v = Vector3::dotProduct(dir, qVector) * inverseDeterminate;
	if (v < 0 || u + v > 1) return false;

	t = Vector3::dotProduct(-triangle.EdgeCA, qVector) * inverseDeterminate;

	tempHit.t = t;
	tempHit.u = u;
	tempHit.v = v;
	tempHit.triangle = triangle;
	tempHit.normal = Vector3::normalize(normals[0] * (1-u-v)+ normals[1] * u + normals[2] * v);
	tempHit.text = text[1];
	float temp = Vector3::magnitude(tempHit.normal);
	_Hit = tempHit;


	return true;
}

/// Takes a Mesh, an origin and a direction
/// passes all the triangles one by one to the calc hit method to see if the given triangle was hit
/// if a triangle was hit the info is stored in the &_Hit and the Method returns true
bool MathOli::shootRay(Vector3 origin, Vector3 dir, Mesh mesh, RayHit &_Hit) {

	std::vector<Triangle> tempList = std::vector<Triangle>();
	std::vector<RayHit> hits = std::vector<RayHit>();

	RayHit tempHit = RayHit();

	for (int x = 0; x < mesh.Faces.size(); x++)
	{
		Triangle temp = Triangle(mesh.Vertices.at(mesh.Faces.at(x).VertexA-1),
			mesh.Vertices.at(mesh.Faces.at(x).VertexB-1),
			mesh.Vertices.at(mesh.Faces.at(x).VertexC-1)
			);
		Vector3 normals[3];
		normals[0] = mesh.Normals.at(mesh.Faces.at(x).NormalA-1);
		normals[1] = mesh.Normals.at(mesh.Faces.at(x).NormalB-1);
		normals[2] = mesh.Normals.at(mesh.Faces.at(x).NormalC-1);
		Vector3 text[3];
		text[0] = mesh.TextureCoords.at(mesh.Faces.at(x).TextCoA - 1);
		text[1] = mesh.TextureCoords.at(mesh.Faces.at(x).TextCoB - 1);
		text[2] = mesh.TextureCoords.at(mesh.Faces.at(x).TextCoC - 1);

		if (MathOli::calcHit(origin, dir, temp, tempHit,normals, text))
		{
			tempList.push_back(temp);
			hits.push_back(tempHit);
		}
	}


	float tempT = std::numeric_limits<float>::max();
	int choice = std::numeric_limits<int>::max();
	for (int i = 0; i < tempList.size(); i++)
	{
		if (hits[i].t < tempT)
		{
			tempT = hits[i].t;
			choice = i;
		}
	}

	if (tempList.size() > 0)
	{
		tempHit.t = hits[choice].t;
		tempHit.u = hits[choice].u;
		tempHit.v = hits[choice].v;
		tempHit.triangle = tempList[choice];
		tempHit.normal = hits[choice].normal;
		tempHit.text = hits[choice].text;
		_Hit = tempHit;
		return true;
	}
	else
		return false;
}

/// Linearly interpolated between two Values
/// Used to calculate the scattering of the Rays shot from the Camera position
float MathOli::lerpValues(const float value1, const float value2, const float _lerpValue){
	return value1 + (value2 - value1) * _lerpValue;
}

/// Linearly interpolated between two Vectors
/// Also not used in the Project :)
Vector3 MathOli::lerpV3(const Vector3 &A, const Vector3 &B, float _lerpValue) {
	return Vector3(lerpValues(A.x, B.x, _lerpValue), lerpValues(A.y,B.y, _lerpValue), lerpValues(A.z, B.z, _lerpValue));
}

