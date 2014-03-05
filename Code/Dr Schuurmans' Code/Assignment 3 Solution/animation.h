#include <iostream>
#include <vector>
#include <Eigen/Dense>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "mesh.h"
#include "skeleton.h"
#include "motion.h"
#include "attachment.h"


#ifndef ANIMATION_H
#define ANIMATION_H

using namespace std;
using namespace Eigen;

struct mesh;
struct joint;
struct skeleton;
struct motion;
struct attachment;


struct animation
{
	mesh* objp;
	skeleton* skelp;
	motion* motp;
	attachment* attp;

	// after compile():
	Vector3f** timeVertices;
	Vector3f** timeNormals;
	Vector3f** timeBoneVertices;

	// animation control
	double loopTime, loopFrac;
	long startTick;
	bool paused;
	bool interpolation;


	// member functions

	animation(mesh* objref, skeleton* skelref, motion* motref, attachment* attref,
	          bool pause, bool interp) : 
		objp(objref), skelp(skelref), motp(motref), attp(attref), 
		paused(pause), interpolation(interp) {}


	void pause()
	{
		paused = true;
	}


	void play()
	{
		paused = false;
		startTick = glutGet(GLUT_ELAPSED_TIME);
	}


	void glIdle()
	{
		if (!paused) {
			long currTick = glutGet(GLUT_ELAPSED_TIME);
			long ticks = currTick - startTick;
			startTick = currTick;
			loopFrac += (double)ticks / 1000.0 / loopTime;
			loopFrac -= floor(loopFrac); // wrap frac into [0,1)
		}
		glutPostRedisplay();
	}


	void timerInitialize();
	void timerSpeedUp(double);
	void timerSlowDown(double);
	void timerStepForward();
	void timerStepBackward();
	void compileAnimation();
	void compileSubTree(joint*, Affine3f, Vector3f, Quaternionf, int, int*);
	void glDrawSkeleton();
	void glDrawMesh();
	void writeFrame(char*, int);

};

#endif
