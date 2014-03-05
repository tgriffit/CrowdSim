
// personViewer <mesh.obj> <motion.bvh>

// CMPUT 411/511 Assignment 3 solution

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <Eigen/Dense>

#include "light.h"
#include "camera.h"
#include "mesh.h"
#include "skeleton.h"
#include "motion.h"
#include "attachment.h"
#include "animation.h"

using namespace Eigen;


int main(int, char**);
void setup(char*, char*);
void drawScene(void);
void idle(void);
void resize(int, int);
void keyInput(unsigned char, int, int);


/* global variables */
light lit(1.0);
camera cam;
mesh obj;
skeleton skel;
motion mot;
attachment att(&obj, &skel);
animation anim(&obj, &skel, &mot, &att, true, false);


int main(int argc, char** argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("personViewer");
	if (argc < 3) {
		cerr << "Error: Usage: personViewer <meshfile.obj> <motionfile.bvh>" <<endl;
		exit(1);
	}
	setup(argv[1], argv[2]); 
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);  
	glutKeyboardFunc(keyInput);
	glutIdleFunc(idle);
	glutMainLoop(); 
	return 0;  
}



void setup(char* objFileName, char* bvhFileName) 
{
	// mesh
	obj.readObjFile(objFileName);
	obj.normalize();
	obj.glCreateDisplayList();
	obj.compile();

	// skeleton
	skel.readBvhSkeleton(bvhFileName);
	skel.determineRadius();
	skel.recoverBones();

	// attachment
	att.distancesVisibility();
	att.connectionValues();
	att.adjacencyLaplacian();
	att.attachmentWeights(1.0);

	// motion
	mot.readBvhMotion(bvhFileName, skel);
	mot.determineRange(skel.translationIndices);

	// animation
	anim.compileAnimation();
	anim.timerInitialize();

	// camera
	cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 200.0);
	cam.positionMotion(mot.range, skel.radius);

	// gl
	glClearColor(0.0, 0.0, 0.0, 0.0);  
	glEnable(GL_DEPTH_TEST);
}


void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// camera view volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cam.glVolume();

	// camera position
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	cam.glPosition(); 

	// draw skeleton
	lit.glDisableLight();
	glColor3f(1.0, 0.0, 0.0);
	anim.glDrawSkeleton();

	// set mesh drawing type, color and material
	if (!obj.solid) { // wireframe
		glDisable(GL_CULL_FACE);
		lit.glDisableLight();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor4fv(obj.material);
	} else { // solid
		if (obj.solid == 1) 
			glShadeModel(GL_FLAT);
		else 
			glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_LINE);
		lit.glEnableLight();
		obj.glMaterial();
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	// draw mesh
	anim.glDrawMesh();

	glutSwapBuffers();
}


void idle(void) 
{ 
	anim.glIdle(); 
}


void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
}


void keyInput(unsigned char key, int x, int y)
{
	switch(key) 
	{
		case 'q': exit(0); break; // quit
		case 'w': 
			obj.writeObjFile("output.obj"); 
			skel.writeBvhSkeleton("output.bvh");
			mot.writeBvhMotion("output.bvh");
			att.writeMatrixXf(att.D, "D.out");
			att.writeMatrixXi(att.V, "V.out");
			att.writeVectorXf(att.h, "h.out");
			att.writeMatrixXf(att.C, "C.out");
			att.writeSparseMatrixXi(att.L, "L.out");
			att.writeMatrixXf(att.W, "W.out");
			break;
		case 'p': anim.play();  break;
		case 'P': anim.pause(); break;
		case 'f': anim.timerStepForward();  break;
		case 'F': anim.timerStepBackward(); break;
		case 'd': cam.xTransl(1.0);    break;
		case 'D': cam.xTransl(-1.0);   break;
		case 'c': cam.yTransl(1.0);    break;
		case 'C': cam.yTransl(-1.0);   break;
		case 'i': cam.zTransl(1.0);    break;
		case 'I': cam.zTransl(-1.0);   break;
		case 't': cam.xRotate(10.0);   break;
		case 'T': cam.xRotate(-10.0);  break;
		case 'a': cam.yRotate(10.0);   break;
		case 'A': cam.yRotate(-10.0);  break;
		case 'l': cam.zRotate(10.0);   break;
		case 'L': cam.zRotate(-10.0);  break;
		case 's': obj.solid = (obj.solid+1)%3; break; // toggle wire, flat, smooth
		case 'x': // reset
			anim.timerInitialize();
			cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 200.0);
			cam.positionMotion(mot.range, skel.radius);
			obj.reset();
			break;
		case '-': // slow down animation rate by 10 fps
			anim.timerSlowDown(10.0);
			break;
		case '+': // speed up animation rate by 10 fps
			anim.timerSpeedUp(10.0);
			break;
		default: break;
	}
	glutPostRedisplay();
}


