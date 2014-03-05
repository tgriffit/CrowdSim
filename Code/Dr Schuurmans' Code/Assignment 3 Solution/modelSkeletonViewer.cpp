
// modelSkeletonViewer <mesh.obj> <motion.bvh>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <Eigen/Dense>

#include "camera.h"
#include "mesh.h"
#include "skeleton.h"
#include "motion.h"
#include "attachment.h"


using namespace Eigen;


int main(int, char**);
void setup(char*, char*);
void drawScene(void);
void resize(int, int);
void keyInput(unsigned char, int, int);
void specialKeyInput(int, int, int);


/* global variables */
camera cam;
mesh obj;
skeleton skel;
motion mot;
attachment att(&obj, &skel);
int highlightBone(0);
bool highlightMode = false;


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("modelSkeletonViewer");
	if (argc < 3) {
		cerr << "Error: Usage: modelSkeletonViewer <meshfile.obj> <motionfile.bvh>" 
		     << endl;
		exit(1);
	}
	setup(argv[1], argv[2]);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
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

	// camera
	cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 200.0);
	cam.transl[2] = -1.0;

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

	// draw mesh
	obj.glPosition(Vector3f::Zero());
	att.glDrawMeshAttach(highlightMode, highlightBone);

	// draw skeleton
	skel.glDrawBones(highlightBone);

	glutSwapBuffers();
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
		case 'd': cam.xTransl(0.05);  break;
		case 'D': cam.xTransl(-0.05); break;
		case 'c': cam.yTransl(0.05);  break;
		case 'C': cam.yTransl(-0.05); break;
		case 'i': cam.zTransl(0.05);  break;
		case 'I': cam.zTransl(-0.05); break;
		case 't': cam.xRotate(5.0);   break;
		case 'T': cam.xRotate(-5.0);  break;
		case 'a': cam.yRotate(5.0);   break;
		case 'A': cam.yRotate(-5.0);  break;
		case 'l': cam.zRotate(5.0);   break;
		case 'L': cam.zRotate(-5.0);  break;
		case 'x': // reset
			obj.reset();
			cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 100.0);
			cam.transl[2] = -1.0;
			highlightBone = 0;
			highlightMode = false;
			break;
		case 'v': highlightMode = !highlightMode;  break;
		case '+':
			highlightBone = (highlightBone + 1) % skel.bones.size();
			break;
		case '-':
			highlightBone = (!highlightBone) ? skel.bones.size()-1 : highlightBone-1;
			break;
		default: break;
	}
	glutPostRedisplay();
}


