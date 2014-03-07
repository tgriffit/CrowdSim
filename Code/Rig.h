#pragma once


#include "Model.h"
#include "Utilities.h"

struct Rig
{
	Model obj;	// TODO: make static once other things work
	//static Skeleton skeleton;
	//static vector<Animation> animations;


	void readRigFile(string filename);
	//Person* spawnInstance(Vector3f loc, Vector3f dir);		// Spawns a person at the specified location facing the specified direction
};