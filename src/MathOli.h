#include"Triangle.h"
#include<vector>

/// struct containing information on successfull Ray Hit with a triangle
struct RayHit
{
public:
	float t, u, v;
	Triangle triangle;
	Vector3 normal;
	Vector3 text;

	RayHit();
};

/// Class containing methods for calculating Ray/Triangle intersection
class MathOli
{

public:
	static const float  kEpsilon;

	static bool shootRay(Vector3 origin, Vector3 dir, Mesh mesh, RayHit &_Hit);

	static bool calcHit(Vector3 origin, Vector3 dir, Triangle triangle, RayHit&_Hit, Vector3 normals[3], Vector3 text[3]);
	static float lerpValues(const float value1, const float value2, const float _lerpValue);
	static Vector3 lerpV3(const Vector3 &A, const Vector3 &B, float _lerpValue);
};

/// Class defining a 4x4Matrix
/// Most importantly used for the lookAt Method
class Matrix4x4 {
public:
	Matrix4x4() {}
	const float* operator [] (uint8_t i) const { return x[i]; }
	float* operator [] (uint8_t i) { return x[i]; }
	float x[4][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };

	/// Not used in the Project
	/// did i do this for fun? :D
	Matrix4x4 transpose() const
	{
		Matrix4x4 transpMat;
		for (uint8_t i = 0; i < 4; ++i) {
			for (uint8_t j = 0; j < 4; ++j) {
				transpMat[i][j] = x[j][i];
			}
		}

		return transpMat;
	}

	/// Method implementing the Matrix multiplication
	/// 
	Vector3 multVecMatrix(const Vector3 &src)const
	{
		float a, b, c, w;
		Vector3 temp;

		a = src.x * x[0][0] + src.y * x[1][0] + src.z * x[2][0] + x[3][0];
		b = src.x * x[0][1] + src.y * x[1][1] + src.z * x[2][1] + x[3][1];
		c = src.x * x[0][2] + src.y * x[1][2] + src.z * x[2][2] + x[3][2];
		w = src.x * x[0][3] + src.y * x[1][3] + src.z * x[2][3] + x[3][3];

		temp.x = a / w;
		temp.y = b / w;
		temp.z = c / w;

		return temp;
	}

	/// lookAt Method used for Camera Targeting
	// Calculates a 4x4 Matrix based on a Camera Position "from" and Target Position "to"
	// Last parameter is used to calculate the right Vector from the local Camera space
	static Matrix4x4 lookAt(const Vector3& from, const Vector3& to, const Vector3& tmp = Vector3(0, 1, 0)) {
		Vector3 forward = Vector3::normalize(from - to);
		Vector3 right = Vector3::crossProduct(Vector3::normalize(tmp), forward);
		Vector3 up = Vector3::crossProduct(forward, right);

		Matrix4x4 camToWorld;

		camToWorld[0][0] = right.x;
		camToWorld[0][1] = right.y;
		camToWorld[0][2] = right.z;
		camToWorld[1][0] = up.x;
		camToWorld[1][1] = up.y;
		camToWorld[1][2] = up.z;
		camToWorld[2][0] = forward.x;
		camToWorld[2][1] = forward.y;
		camToWorld[2][2] = forward.z;

		camToWorld[3][0] = from.x;
		camToWorld[3][1] = from.y;
		camToWorld[3][2] = from.z;

		return camToWorld;
	}
};