
// modelSolidViewer <mesh.obj>
// same as modelViewer, but toggles between wireframe, flat and smooth shading

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <Eigen/Dense>

#include "light.h"
#include "camera.h"
#include "mesh.h"

using namespace Eigen;


int main(int, char**);
void setup(char*);
void drawScene(void);
void resize(int, int);
void keyInput(unsigned char, int, int);
void specialKeyInput(int, int, int);


/* global variables */
light lit(0.1);
camera cam;
mesh obj;
Vector3f initialObjPosition(0.0, 0.0, -1.0);


int main(int argc, char** argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("modelSolidViewer");
	setup(argv[1]); 
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);  
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutMainLoop(); 
	return 0;
}


void setup(char* fileName) 
{
	obj.readObjFile(fileName);
	obj.normalize();
	obj.glCreateDisplayList();
	cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 100.0);
	glClearColor(0.0, 0.0, 0.0, 0.0);  
	glEnable(GL_DEPTH_TEST);
}


void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cam.glVolume(); // camera view volume

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	cam.glPosition(); // camera transformation

	// set drawing type, color, material
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

	obj.glPosition(initialObjPosition); // object transformation
	glCallList(obj.displayList); // draw object

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
		case 'q': exit(0);             break; // quit
		case 'w': obj.writeObjFile("output.obj"); break;
		case 'n': obj.zTransl(-0.05); break;
		case 'N': obj.zTransl(0.05);  break;
		case 'p': obj.xRotate(-5.0);  break;
		case 'P': obj.xRotate(5.0);   break;
		case 'y': obj.yRotate(-5.0);  break;
		case 'Y': obj.yRotate(5.0);   break;
		case 'r': obj.zRotate(-5.0);  break;
		case 'R': obj.zRotate(5.0);   break;
		case 'v': cam.type = ortho;   break; // toggle camera type
		case 'V': cam.type = persp;   break;
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
		case 's': obj.solid = (obj.solid+1)%3; break; // toggle wire, flat, smooth
		case 'x': // reset
			obj.reset();
			cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 100.0);
			break;
		default: break;
	}
	glutPostRedisplay();
}


void specialKeyInput(int key, int x, int y)
{
	switch(key) 
	{
		case GLUT_KEY_LEFT:  obj.xTransl(-0.05); break;
		case GLUT_KEY_RIGHT: obj.xTransl(0.05);  break;
		case GLUT_KEY_DOWN:  obj.yTransl(-0.05); break;
		case GLUT_KEY_UP:    obj.yTransl(0.05);  break;
		default: break;
	}
	glutPostRedisplay();
}


