
#include "animation.h"


using namespace std;
using namespace Eigen;


void animation::timerInitialize()
{
	paused = true;
	startTick = glutGet(GLUT_ELAPSED_TIME);
	loopFrac = 0.0;
	loopTime = motp->defaultGapTime * (double)motp->sequence.size();
}


void animation::timerSpeedUp(double fps)
{
	loopTime /= (1.0 + fps*loopTime/(double)motp->sequence.size());
}


void animation::timerSlowDown(double fps)
{
	loopTime /= (1.0 - fps*loopTime/(double)motp->sequence.size());
}


void animation::timerStepForward()
{
	double timeIdx = loopFrac * (double)motp->sequence.size();
	int t = (int)round(timeIdx);
	t = (t >= motp->sequence.size()-1) ? 0 : t+1;
	loopFrac = (double)t/(double)motp->sequence.size();
}


void animation::timerStepBackward()
{
	double timeIdx = loopFrac * (double)motp->sequence.size();
	int t = (int)round(timeIdx);
	t = (t <= 0) ? motp->sequence.size()-1 : t-1;
	loopFrac = (double)t/(double)motp->sequence.size();
}


void animation::compileAnimation()
{
	timeVertices     = new Vector3f*[motp->sequence.size()];
	timeNormals      = new Vector3f*[motp->sequence.size()];
	timeBoneVertices = new Vector3f*[motp->sequence.size()];

	for (int t = 0; t < motp->sequence.size(); t++) {

		cout << "compiling frame " << t << endl;

		timeVertices[t]     = new Vector3f[objp->vertices.size()]();
		timeNormals[t]      = new Vector3f[objp->vertices.size()]();
		timeBoneVertices[t] = new Vector3f[skelp->vertices.size()]();

		for (int i = 0; i < objp->vertices.size(); i++) {
			timeVertices[t][i] = Vector3f::Zero();
			timeNormals [t][i] = Vector3f::Zero();
		}

		double loopFracIdx = t/(double)motp->sequence.size();
		skelp->pose(*motp, loopFracIdx, false);
		int j = -1;
		Affine3f trans = Affine3f::Identity()*Translation3f(skelp->root->transl);
		Vector3f offset(Vector3f::Zero());
		Quaternionf quat(Quaternionf::Identity());

		compileSubTree(skelp->root, trans, offset, quat, t, &j);
	}
}


void animation::compileSubTree(joint* curr, Affine3f trans, Vector3f offset,
                               Quaternionf quat, int t, int* jp)
{
	(*jp)++;
	timeBoneVertices[t][*jp] = trans*(skelp->vertices[*jp] - offset);

	if (curr->type != rootJoint) {
		for (int i = 0; i < objp->vertices.size(); i++) {
				float weight = (*attp->W)(i, *jp-1);
				if (weight >= 0.01) {
					timeVertices[t][i] += trans*(objp->vertices[i] - offset)*weight;
					timeNormals[t][i] += quat._transformVector(objp->normals[i])*weight;
				}
		}
	}

	Affine3f currTrans(trans);
	currTrans *= Translation3f(curr->offset);
	currTrans *= curr->quaternion;

	Vector3f currOffset(offset);
	currOffset += curr->offset;

	Quaternionf currQuat(quat);
	currQuat *= curr->quaternion;

	for (int c = 0; c < curr->children.size(); c++)
		compileSubTree(curr->children[c], currTrans, currOffset, currQuat, t, jp);
}


void animation::glDrawSkeleton()
{
	double timeIdx = loopFrac * (double)motp->sequence.size();
	unsigned int t = (unsigned int)round(timeIdx);
	t = min(t, (unsigned int)motp->sequence.size() - 1);

	for (int j = 0; j < skelp->bones.size(); j++) {
		int v0 = skelp->bones[j][0];
		int v1 = skelp->bones[j][1];
		glBegin(GL_LINES);
			glVertex3fv(timeBoneVertices[t][v0].data());
			glVertex3fv(timeBoneVertices[t][v1].data());
		glEnd();
	}
}


void animation::glDrawMesh()
{
	double timeIdx = loopFrac * (double)motp->sequence.size();
	unsigned int t = (unsigned int)round(timeIdx);
	t = min(t, (unsigned int)motp->sequence.size() - 1);

	for (int f = 0; f < objp->faceVertices.size(); f++) {
		glBegin(GL_TRIANGLE_FAN);
			for (int j = 0; j < objp->faceVertices[f].size(); j++) {
				int v = objp->faceVertices[f][j];
				if (objp->normals.size()) glNormal3fv(timeNormals[t][v].data());
				glVertex3fv(timeVertices[t][v].data());
			}
		glEnd();
	}
}


void animation::writeFrame(char* fileName, int t)
{
	ofstream outfile(fileName);
	if (!outfile) {
		cerr << "Error: unable to open output file: " << fileName << endl;
		exit(1);
	}
	for (int i = 0; i < objp->vertices.size(); i++) {
		outfile << i << " ";
		for (int j = 0; j < 3; j++)
			outfile << timeVertices[t][i][j] << " ";
		outfile << endl;
	}
}
