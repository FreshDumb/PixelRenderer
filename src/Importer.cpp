#include"Importer.h"

/// Method to skip white spaces
std::istream &SkipWS(std::istream &In) {
	while (isspace(In.peek()))
		(void)In.get();
	return In;
}

Vector3 ImportVertex(std::istream &In) {
	char c = In.get();
	Vector3 v;
	In >> v.x >> v.y >> v.z;
	return v;
}
Vector3 ImportNormal(std::istream &In) {
	char c = In.get();
	In.get();
	Vector3 v;
	In >> v.x >> v.y >> v.z;
	return v;
}
Vector3 ImportTextureCoords(std::istream &In) {
	char c = In.get();
	Vector3 v;
	In >> v.x >> v.y;
	if (SkipWS(In).peek() == 'v') {
		v.z = 0;
	}
	else if(In.peek() != 'f') {
		In >> v.z;
	}
	return v;
}

void ImportVertexArray(std::istream &In, VectorArray &vs, VectorArray &ns, VectorArray &vx) {
	while (SkipWS(In).peek() == 'v') {
		char c = In.get();
		c = In.peek();
		if ((In).peek() != 'n' && (In).peek() != 't') {
			vs.push_back(ImportVertex(In));
		}
		else if ((In).peek() == 'n') {
			ns.push_back(ImportNormal(In));
		}
		else if ((In).peek() == 't') {
			vx.push_back(ImportTextureCoords(In));
		}
	}
}


Face ImportFace(std::istream &In) {
	char c = In.get();
	Face f;
	In >> f.VertexA >> c >> f.TextCoA >> c >> f.NormalA >>
		f.VertexB >> c >> f.TextCoB >> c >> f.NormalB >>
		f.VertexC >> c >> f.TextCoC >> c >> f.NormalC;
	return f;
}
void ImportFaceArray(std::istream &In, FaceArray &fs) {
	while (SkipWS(In).peek() == 'f')
		fs.push_back(ImportFace(In));
}

void Mesh::ImportMesh(std::istream &In, Mesh& m) {
	ImportVertexArray(In, m.Vertices, m.Normals, m.TextureCoords);
	ImportFaceArray(In, m.Faces);
}
