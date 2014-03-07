#pragma once

#include <iostream>
#include <fstream> 
#include <sstream> 
#include <string>
#include <cfloat>
#include <cmath>
#include <vector>
#include <GL/glut.h>

#include "Utilities.h"

struct Model
{
	vector<Vector3f> vertices, textures, normals;
	vector< vector<int> > faceVertices, faceTextures, faceNormals;

	float meshScale;			// object scale divisor
	Vector3f center;			// object center
	Vector3f translation;		// object translation
	Quaternionf quaternion;		// object orientation
	unsigned int displayList;	// display list
	float material[4];
	float shininess[1];
	int solid;					// toggle for wireframe, flat, smooth shading

	// after compile():
	float* verticesCompiled;	// array[3*numVertices]
	int* trianglesCompiled;		// array[3*numFaces]
	float* diameters;			// array[numFaces]


	Model()
	{
		translation << 0, 0, 0;
		quaternion = Quaternionf::Identity();
		material[0] = material[1] = material[2] = 0.8;
		material[3] = 1.0;
		shininess[0] = 0.0;
		solid = 0;
	}


	void reset()
	{
		translation << 0, 0, 0;
		quaternion = Quaternionf::Identity();
		solid = 0;
	}

	
	// place mesh in specified position+orientation in openGL
	void glPosition(Vector3f initialPosition) 
	{
		glTranslatef(translation[0], translation[1], translation[2]);
		glTranslatef(initialPosition[0], initialPosition[1], initialPosition[2]);
		float w = max( (float)-1, min( (float)1, quaternion.w() ) ); // |w|<=1
		float angle = 2*acos(w)*RAD2DEG;
		glRotatef(angle, quaternion.x(), quaternion.y(), quaternion.z());
		glScalef(meshScale, meshScale, meshScale);
		glTranslatef(-center[0], -center[1], -center[2]);
	}


	void glMaterial()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, material);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material);
		glMaterialfv(GL_FRONT, GL_SPECULAR, material);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	}

	void readObjFile(const char* filename);
	void normalize();
	void glCreateDisplayList();
	void compile();
};