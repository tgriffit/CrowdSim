
// motionViewer <motion.bvh>

// CMPUT 411/511 Assignment 2 solution
// Note: camera key-controls altered to match Assignment 1


#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "camera.h"
#include "skeleton.h"
#include "motion.h"


int main(int, char**);
void setup(char*);
void drawScene(void);
void idle(void);
void resize(int, int);
void keyInput(unsigned char, int, int);


/* Global variables */

camera cam;
skeleton skel;
motion mot;

// animation control
double loopTime, loopFrac;
long startTick;
bool paused = true;
bool interpolation = true;


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("motionViewer");
	setup(argv[1]); 
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}


void setup(char *fileName) 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	skel.readBvhSkeleton(fileName);
	skel.determineRadius();
	mot.readBvhMotion(fileName, skel);
	mot.determineRange(skel.translationIndices);

	// animation control
	loopTime = mot.defaultGapTime * (double)mot.sequence.size();
	loopFrac = 0.0;
	startTick = glutGet(GLUT_ELAPSED_TIME);

	// camera
	cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 200.0);
	cam.positionMotion(mot.range, skel.radius);

	// camera view volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cam.glVolume(); 
}


void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	cam.glPosition();

	// skeleton
	glColor3f(1.0, 1.0, 1.0);
	skel.pose(mot, loopFrac, interpolation);
	skel.glDrawSubTree(skel.root);

	glutSwapBuffers();
}


void idle(void)
{
	if (!paused) {
		long currTick = glutGet(GLUT_ELAPSED_TIME);
		long ticks = currTick - startTick;
		startTick = currTick;
		loopFrac += (double)ticks / 1000.0 / loopTime;
		loopFrac -= floor(loopFrac); // wrap frac into [0,1)
		glutPostRedisplay();
	}
}


void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
}


void keyInput(unsigned char key, int x, int y)
{
	switch(key) 
	{
		case 'q': // quit
			exit(0); 
			break; 
		case 'w': // write bvh file
			skel.writeBvhSkeleton("output.bvh");
			mot.writeBvhMotion("output.bvh");
			break;
		case 'p': // play
			paused = false;
			startTick = glutGet(GLUT_ELAPSED_TIME);
			break;
		case 'P': // pause
			paused = true;
			break;
		case 'd': cam.xTransl(1.0);   break;
		case 'D': cam.xTransl(-1.0);  break;
		case 'c': cam.yTransl(1.0);   break;
		case 'C': cam.yTransl(-1.0);  break;
		case 'i': cam.zTransl(1.0);   break;
		case 'I': cam.zTransl(-1.0);  break;
		case 't': cam.xRotate(10.0);  break;
		case 'T': cam.xRotate(-10.0); break;
		case 'a': cam.yRotate(10.0);  break;
		case 'A': cam.yRotate(-10.0); break;
		case 'l': cam.zRotate(10.0);  break;
		case 'L': cam.zRotate(-10.0); break;
		case 'x': // reset
			paused = true;
			startTick = glutGet(GLUT_ELAPSED_TIME);
			loopFrac = 0.0;
			loopTime = mot.defaultGapTime * (double)mot.sequence.size();
			cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 200.0);
			cam.positionMotion(mot.range, skel.radius);
			break;
		case '-': // slow down animation rate by 10 fps
			loopTime /= (1.0 - 10.0*loopTime/(double)mot.sequence.size());
			break;
		case '+': // speed up animation rate by 10 fps
			loopTime /= (1.0 + 10.0*loopTime/(double)mot.sequence.size());
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

