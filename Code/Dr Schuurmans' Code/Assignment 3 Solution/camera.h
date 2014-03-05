#include <cmath>
#include <vector>
#include <Eigen/Dense>

#ifndef CAMERA_H
#define CAMERA_H

#define DEG2RAD_CAM 0.017453292519943 /* rad = deg*PI/180 = deg*DEG2RAD */
#define RAD2DEG_CAM 57.29577951308232 /* deg = rad*180/PI = rad*RAD2DEG */

using namespace std;
using namespace Eigen;


enum cameraType {ortho, persp};

struct camera
{
	cameraType type;
	Vector2f xView; // view plane left, right
	Vector2f yView; // view plane down, up
	Vector2f zView; // view volume -near, -far
	Vector3f transl; // translation
	Quaternionf quaternion; // orientation


	// member functions

	// initializer not constructor, since we also want to reset after creation
	void initialize(cameraType tp,
	       float x0, float x1, float y0, float y1, float z0, float z1) 
	{
		type = tp;
		xView << x0, x1;
		yView << y0, y1;
		zView << z0, z1;
		transl = Vector3f::Zero();
		quaternion = Quaternionf::Identity();
	}


	void glVolume() // define view volume in openGL
	{
		if (type == ortho)
			glOrtho(xView[0], xView[1], yView[0], yView[1], zView[0], zView[1]);
		else
			glFrustum(xView[0], xView[1], yView[0], yView[1], zView[0], zView[1]);
	}


	void glPosition() // place camera in specified position+orientation in openGL
	{
		float w = max( (float)-1, min( (float)1, quaternion.w() ) ); // |w|<=1
		float angle = 2*acos(w)*RAD2DEG_CAM;
		glRotatef(angle, quaternion.x(), quaternion.y(), quaternion.z()); // orient
		glTranslatef(transl[0], transl[1], transl[2]); // position
	}


	void xTransl(float trans) // dolly along camera x-axis
	{
		Vector3f axis = quaternion.conjugate()._transformVector(Vector3f::UnitX());
		transl += trans*axis;
	}


	void yTransl(float trans) // crane along camera y-axis
	{
		Vector3f axis = quaternion.conjugate()._transformVector(Vector3f::UnitY());
		transl += trans*axis;
	}


	void zTransl(float trans) // zoom along camera z-axis
	{
		Vector3f axis = quaternion.conjugate()._transformVector(Vector3f::UnitZ());
		transl += trans*axis;
	}


	void xRotate(float angleDeg) // tilt around camera x-axis
	{
		float angleQuat = angleDeg/2.0*DEG2RAD_CAM;
		Vector4f newRotation;
		newRotation << Vector3f::UnitX() * sin(angleQuat), cos(angleQuat);
		quaternion = (Quaternionf)newRotation * quaternion;
	}


	void yRotate(float angleDeg) // pan around camera y-axis
	{
		float angleQuat = angleDeg/2.0*DEG2RAD_CAM;
		Vector4f newRotation;
		newRotation << Vector3f::UnitY() * sin(angleQuat), cos(angleQuat);
		quaternion = (Quaternionf)newRotation * quaternion;
	}


	void zRotate(float angleDeg) // roll around camera z-axis
	{
		float angleQuat = angleDeg/2.0*DEG2RAD_CAM;
		Vector4f newRotation;
		newRotation << Vector3f::UnitZ() * sin(angleQuat), cos(angleQuat);
		quaternion = (Quaternionf)newRotation * quaternion;
	}


	void positionMotion(vector<Vector3f> motionRange, float radius)
	{
		transl = -motionRange[0];
		transl(2) -= motionRange[1].maxCoeff() + radius;
	}

};

#endif
