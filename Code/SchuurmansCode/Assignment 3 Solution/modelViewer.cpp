
// modelViewer <mesh.obj>

// CMPUT 411/511 Assignment 1 solution


#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <Eigen/Dense>

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
mesh obj;
camera cam;
Vector3f initialPosition(0.0, 0.0, -1.0);
bool fog = true;
const float fogColor[4] = {0.0, 0.0, 0.0, 0.0};


int main(int argc, char** argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("modelViewer");
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
	obj.glPosition(initialPosition); // object transformation

	if (fog) { // set fog
		glEnable(GL_FOG);
		glHint(GL_FOG_HINT, GL_NICEST);
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogi(GL_FOG_START, 1.0);
		glFogi(GL_FOG_END, 5.0);
	} else
		glDisable(GL_FOG);

	// draw model
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4fv(obj.material);
	glCallList(obj.displayList);
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
		case 'q': exit(0);            break; // quit
		case 'w': obj.writeObjFile("output.obj"); break;
		case 'n': obj.zTransl(-0.1);  break;
		case 'N': obj.zTransl(0.1);   break;
		case 'p': obj.xRotate(-10.0); break;
		case 'P': obj.xRotate(10.0);  break;
		case 'y': obj.yRotate(-10.0); break;
		case 'Y': obj.yRotate(10.0);  break;
		case 'r': obj.zRotate(-10.0); break;
		case 'R': obj.zRotate(10.0);  break;
		case 'v': cam.type = ortho;   break; // toggle camera type
		case 'V': cam.type = persp;   break;
		case 'd': cam.xTransl(0.1);   break;
		case 'D': cam.xTransl(-0.1);  break;
		case 'c': cam.yTransl(0.1);   break;
		case 'C': cam.yTransl(-0.1);  break;
		case 'i': cam.zTransl(0.1);   break;
		case 'I': cam.zTransl(-0.1);  break;
		case 't': cam.xRotate(10.0);  break;
		case 'T': cam.xRotate(-10.0); break;
		case 'a': cam.yRotate(10.0);  break;
		case 'A': cam.yRotate(-10.0); break;
		case 'l': cam.zRotate(10.0);  break;
		case 'L': cam.zRotate(-10.0); break;
		case 'f': fog = false;        break; // toggle fog off
		case 'F': fog = true;         break; // toggle fog on
		case 'x': // reset
			obj.reset();
			cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 100.0);
			fog = false;
			break;
		default: break;
	}
	glutPostRedisplay();
}


void specialKeyInput(int key, int x, int y)
{
	switch(key) 
	{
		case GLUT_KEY_LEFT:  obj.xTransl(-0.1); break;
		case GLUT_KEY_RIGHT: obj.xTransl(0.1);  break;
		case GLUT_KEY_DOWN:  obj.yTransl(-0.1); break;
		case GLUT_KEY_UP:    obj.yTransl(0.1);  break;
		default: break;
	}
	glutPostRedisplay();
}


