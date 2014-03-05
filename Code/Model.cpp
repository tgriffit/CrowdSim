#include "Model.h"

void Model::readObjFile(const char* filename)
{
	string line, token;
	ifstream file(filename);
	if (!file) 
	{
		error("ERROR: unable to open input file: %s", filename);
	}

	while (getline(file, line)) 
	{
		istringstream stin(line);

		if (!(stin >> token)) continue;

		if (token == "v") 
		{
			Vector3f p;
			stin >> p[0] >> p[1] >> p[2];
			vertices.push_back(p);

		} 
		else if (token == "vt") 
		{
			Vector3f p;
			stin >> p[0] >> p[1] >> p[2];
			textures.push_back(p);

		} 
		else if (token == "vn") 
		{
			Vector3f p;
			stin >> p[0] >> p[1] >> p[2];
			normals.push_back(p);

		} 
		else if (token == "f") 
		{
			vector<int> faceV, faceT, faceN;
			int n;
			while (stin >> n) 
			{
				faceV.push_back(n-1);
				if (stin.get() == '/') 
				{
					if (stin.peek() == '/') 
					{
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
	file.close();
}


void Model::normalize()
{
	Vector3f totals(0, 0, 0);
	Vector3f maxs(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Vector3f mins(FLT_MAX, FLT_MAX, FLT_MAX);

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		for (int j = 0; j < 3; j++) 
		{
			maxs[j] = max(maxs[j], vertices[i][j]);
			mins[j] = min(mins[j], vertices[i][j]);
		}
	}

	center = totals / (float)vertices.size();
	Vector3f scales = maxs - mins;
	float scale = (float)scales.maxCoeff();
	meshScale = 1.0/scale;
}


void Model::glCreateDisplayList()
{
	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);

	for (unsigned int f = 0; f < faceVertices.size(); f++) 
	{
		glBegin(GL_TRIANGLE_FAN);
		for (unsigned int j = 0; j < faceVertices[f].size(); j++) 
		{
			int v = faceVertices[f][j];
			if (normals.size()) glNormal3fv(normals[v].data());
			glVertex3fv(vertices[v].data()); 
		}
		glEnd();
	}
	glEndList();
}


void Model::compile()
{
	verticesCompiled = new float[3*vertices.size()];
	trianglesCompiled = new int[3*faceVertices.size()];
	diameters = new float[faceVertices.size()];

	for (unsigned int i = 0; i < vertices.size(); i++) 
	{
		if (vertices[i].size() != 3) 
		{
			error("ERROR: assumes 3 dimensional vertices");
		}

		verticesCompiled[3*i+0] = vertices[i][0];
		verticesCompiled[3*i+1] = vertices[i][1];
		verticesCompiled[3*i+2] = vertices[i][2];
	}

	for (unsigned int i = 0; i < faceVertices.size(); i++) 
	{
		if (faceVertices[i].size() != 3) 
		{
			error("ERROR: assumes triangular faces");
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