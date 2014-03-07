#include "Simulation.h"

Environment Simulation::env = Environment();
Rig Simulation::rig = Rig();

Vector3f initialObjPosition(0.0, 0.0, -2.0);

void Simulation::startSimulation(int argc, char** argv, string envFile, string rigFile)
{
	env.loadEnvironment(envFile);
	rig.readRigFile(rigFile);

	env.normalize();
	env.glCreateDisplayList();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("CrowdSim");
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);  
	glutKeyboardFunc(keyInput);
	//glutIdleFunc(idle);
	glutMainLoop(); 
}

void Simulation::drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//cam.glVolume(); // camera view volume

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	//cam.glPosition(); // camera transformation

	// set drawing type, color, material
	if (!env.solid) { // wireframe
		glDisable(GL_CULL_FACE);
		//lit.glDisableLight();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor4fv(env.material);
	} else { // solid
		if (env.solid == 1) 
			glShadeModel(GL_FLAT);
		else 
			glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_LINE);
		//lit.glEnableLight();
		env.glMaterial();
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	env.glPosition(initialObjPosition); // object transformation
	glCallList(env.displayList); // draw object

	glutSwapBuffers();
}

void Simulation::resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
}

void Simulation::keyInput(unsigned char key, int x, int y)
{

}

void Simulation::specialKeyInput(int key, int x, int y)
{

}