#include <iostream>
#include <fstream> 
#include <sstream> 
#include <string>
#include <vector>
#include <cfloat>
#include <Eigen/Dense>

#include "bvh_util.h"


#ifndef MOTION_H
#define MOTION_H

using namespace std;
using namespace Eigen;

struct skeleton;


// motion class

struct motion
{
	vector< vector<float> > sequence;
	vector<Vector3f> range;
	double defaultGapTime;

	// member functions
	void readBvhMotion(char*, skeleton);
	vector<Vector3f> motionBounds(Vector3i);
	void writeBvhMotion(char*);

	void determineRange(Vector3i translationIndices) 
	{ 
		range = motionBounds(translationIndices); 
	}
};

#endif

