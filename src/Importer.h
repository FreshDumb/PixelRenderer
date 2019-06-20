#include<vector>
#include"Vector3.h"
#include <fstream>
#include<iostream>

std::istream &SkipWS(std::istream &In); 


///	Importer Methods to load in Mesh from .obj file
/*
Takes a formatted .obj file and converts it to a Mesh Object instance
Loads Vertices, Normals and Texture Coords into separate Arrays
*/

class VectorArray : public std::vector<Vector3>
{
};
Vector3 ImportVertex(std::istream &In);
Vector3 ImportNormal(std::istream &In);
Vector3 ImportTextureCoords(std::istream &In);

class Face
{
public:
	int VertexA, VertexB, VertexC;
	int NormalA, NormalB, NormalC;
	int TextCoA, TextCoB, TextCoC;
};
Face ImportFace(std::istream &In);

class FaceArray : public std::vector<Face>
{
};
void ImportFaceArray(std::istream &In, FaceArray &fs);

/// Mesh Class to store Mesh information
//	Vertices, Normals and TextCoords stored as Vector3
//	Faces stored as Indices to the respective Vectors
class Mesh
{
public:
	Vector3 Position = Vector3();
	VectorArray Vertices;
	VectorArray Normals;
	VectorArray TextureCoords;
	FaceArray Faces;

	void ImportMesh(std::istream &In, Mesh& m);
};
