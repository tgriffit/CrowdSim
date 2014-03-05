
#include "mesh.h"

using namespace std;
using namespace Eigen;


void mesh::readObjFile(char* fileName)
{
	string line, token;
	ifstream infile(fileName);
	if (!infile) {
		cerr << "Error: unable to open input file: " << fileName << endl;
		exit(1);
	}

	while (getline(infile, line)) {
		istringstream stin(line);

		if (!(stin >> token)) continue;

		if (token == "v") {
			Vector3f p;
			stin >> p[0] >> p[1] >> p[2];
			vertices.push_back(p);

		} else if (token == "vt") {
			Vector3f p;
			stin >> p[0] >> p[1] >> p[2];
			textures.push_back(p);

		} else if (token == "vn") {
			Vector3f p;
			stin >> p[0] >> p[1] >> p[2];
			normals.push_back(p);

		} else if (token == "f") {

			vector<int> faceV, faceT, faceN;
			int n;
			while (stin >> n) {
				faceV.push_back(n-1);
				if (stin.get() == '/') {
					if (stin.peek() == '/') {
						stin.get();
						if (stin >> n) faceN.push_back(n-1);
						continue;
					}
					if (stin >> n) faceT.push_back(n-1);
					if (stin.get() == '/' && stin >> n) faceN.push_back(n-1);
				}
			}
			faceVertices.push_back(faceV);
			if (faceT.size()) faceTextures.push_back(faceT);
			if (faceN.size()) faceNormals.push_back(faceN);
		}
	}
	infile.close();
}


void mesh::writeObjFile(char* fileName)
{
	ofstream outfile(fileName);
	if (!outfile) {
		cerr << "Error: unable to open output file: " << fileName << endl;
		exit(1);
	}
	for (int i = 0; i < vertices.size(); i++)
		outfile << "v " << vertices[i][0] << " " << vertices[i][1] 
						<< " " << vertices[i][2] << endl;
	if (textures.size())
		for (int i = 0; i < textures.size(); i++)
			outfile << "vt " << textures[i][0] << " " << textures[i][1] 
							<< " " << textures[i][2] << endl;
	if (normals.size())
		for (int i = 0; i < normals.size(); i++)
			outfile << "vn " << normals[i][0] << " " << normals[i][1] 
							<< " " << normals[i][2] << endl;
	for (int i = 0; i < faceVertices.size(); i++) {
		outfile << "f ";
		for (int j = 0; j < faceVertices[i].size(); j++) {
			outfile << faceVertices[i][j] + 1;
			if (faceTextures.size()) outfile << "/" << faceTextures[i][j] + 1;
			if (faceNormals.size()) outfile << "/" << faceNormals[i][j] + 1;
			outfile << " ";
		}
		outfile << endl;
	}
	outfile.close();
}


void mesh::normalize()
{
	Vector3f totals(0, 0, 0);
	Vector3f maxs(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Vector3f mins(FLT_MAX, FLT_MAX, FLT_MAX);

	for (int i = 0; i < vertices.size(); i++)
		for (int j = 0; j < 3; j++) {
			maxs[j] = max(maxs[j], vertices[i][j]);
			mins[j] = min(mins[j], vertices[i][j]);
		}
	center = totals / (float)vertices.size();
	Vector3f scales = maxs - mins;
	float scale = (float)scales.maxCoeff();
  meshScale = 1.0/scale;
}


void mesh::glCreateDisplayList()
{
	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);

	for (int f = 0; f < faceVertices.size(); f++) {
		glBegin(GL_TRIANGLE_FAN);
		for (int j = 0; j < faceVertices[f].size(); j++) {
			int v = faceVertices[f][j];
			if (normals.size()) glNormal3fv(normals[v].data());
			glVertex3fv(vertices[v].data()); 
		}
		glEnd();
	}
	glEndList();
}


void mesh::compile()
{
	verticesCompiled = new float[3*vertices.size()];
	trianglesCompiled = new int[3*faceVertices.size()];
	diameters = new float[faceVertices.size()];

	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].size() != 3) {
			cerr << "Error: assumes 3 dimensional vertices" << endl;
			exit(1);
		}
		verticesCompiled[3*i+0] = vertices[i][0];
		verticesCompiled[3*i+1] = vertices[i][1];
		verticesCompiled[3*i+2] = vertices[i][2];
	}
	for (int i = 0; i < faceVertices.size(); i++) {
		if (faceVertices[i].size() != 3) {
			cerr << "Error: assumes triangular faces" << endl;
			exit(1);
		}
		trianglesCompiled[3*i+0] = faceVertices[i][0];
		trianglesCompiled[3*i+1] = faceVertices[i][1];
		trianglesCompiled[3*i+2] = faceVertices[i][2];

		float diameter = 0;
		Vector3f p0 = vertices[faceVertices[i][0]];
		Vector3f p1 = vertices[faceVertices[i][1]];
		Vector3f p2 = vertices[faceVertices[i][2]];
		diameter = max(diameter, sqrt((p1 - p0).dot(p1 - p0)));
		diameter = max(diameter, sqrt((p2 - p1).dot(p2 - p1)));
		diameter = max(diameter, sqrt((p0 - p2).dot(p0 - p2)));
		diameters[i] = diameter;
	}
}
