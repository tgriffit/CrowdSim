#pragma once

#include <GL/glut.h>

#include "Utilities.h"
#include "Environment.h"
#include "Rig.h"


class Simulation
{
	static Environment env;
	static Rig rig;

	// OpenGL functions
	static void drawScene();
	static void resize(int w, int h);
	static void keyInput(unsigned char key, int x, int y);
	static void specialKeyInput(int key, int x, int y);

public:
	static void startSimulation(int argc, char** argv, string env, string rig);
};