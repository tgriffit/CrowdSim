#include <iostream>
#include <fstream> 
#include <sstream> 
#include <string>
#include <vector>
#include <cmath>
#include <Eigen/Dense>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "bvh_util.h"
#include "motion.h"


#ifndef SKELETON_H
#define SKELETON_H

#define DEG2RAD_SKE 0.017453292519943 /* rad = deg*PI/180 = deg*DEG2RAD */
#define RAD2DEG_SKE 57.29577951308232 /* deg = rad*180/PI = rad*RAD2DEG */

using namespace std;
using namespace Eigen;

struct motion;


// joint class

enum jointType {rootJoint, regularJoint, endSite};

struct joint
{
	string name;
	jointType type;
	Vector3f offset;
	Vector3f rotate, rotateB;
	Vector3f transl, translB;
	Quaternionf quaternion, quaternionB;
	vector<joint*> children;
	vector<string> channelNames;

	// constructor
	joint(string nm, jointType tp) 
	{
		name = nm;
		type = tp;
		offset = Vector3f::Zero();
		rotate = Vector3f::Zero();
		transl = Vector3f::Zero();
		quaternion = Quaternionf::Identity();
		rotateB = Vector3f::Zero();
		translB = Vector3f::Zero();
		quaternionB = Quaternionf::Identity();
	}
};


// skeleton class

struct skeleton
{
	joint* root;
	float radius;
	vector<float*> channels, channelsB;
	Vector3i translationIndices;
	vector<Vector3f> vertices;
	vector<Vector2i> bones;

	// member functions
	void glDrawSubTree(joint*);
	void pose();
	void pose(motion, double, bool);
	void poseSubTree(joint*, float);
	Quaternionf quaternionFromEulers(Vector3f, vector<string>);
	void recoverBones();
	void recoverSubTree(joint*, Vector3f, int);
	void glDrawBones(int);
	void readBvhSkeleton(char*);
	joint* acquireSubTree(ifstream&, string, jointType);
	float maxBranchLength(joint*);
	void writeBvhSkeleton(char*);
	void outputSubTree(ofstream&, joint*, unsigned int);

	void determineRadius() { radius = maxBranchLength(root); }

};

#endif

