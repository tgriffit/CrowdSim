
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#ifndef LIGHT_H
#define LIGHT_H


struct light
{
	float colorAmbient[4];
	float colorDiffuse[4];
	float colorSpecula[4];
	float position[4];


	// member functions

	light(float intensity)
	{
		colorAmbient[0] = colorAmbient[1] = colorAmbient[2] = 0.0;
		colorDiffuse[0] = colorDiffuse[1] = colorDiffuse[2] = intensity;
		colorSpecula[0] = colorSpecula[1] = colorSpecula[2] = 0.0;
		colorAmbient[3] = colorDiffuse[3] = colorSpecula[3] = 1.0;
		position[0] = position[1] = position[2] = 1.0;
		position[3] = 0.0; // directional
	}


	light(float* amb, float* dif, float* spe, float* pos) 
	{
		for (unsigned int i = 0; i < 4; i++) {
			colorAmbient[i] = amb[i];
			colorDiffuse[i] = dif[i];
			colorSpecula[i] = spe[i];
			position[i] = pos[i];
		}
	}


	void glEnableLight()
	{
		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_AMBIENT,  colorAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE,  colorDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, colorSpecula);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glEnable(GL_LIGHT0);
	}


	void glDisableLight()
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
	

};

#endif
