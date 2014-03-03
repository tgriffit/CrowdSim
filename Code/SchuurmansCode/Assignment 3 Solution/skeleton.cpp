
#include "skeleton.h"

using namespace std;
using namespace Eigen;


void skeleton::glDrawSubTree(joint* current)
{
	glPushMatrix();

	if (current->type == rootJoint)
		glTranslatef(current->transl(0), current->transl(1), current->transl(2));
	else {
		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3fv(current->offset.data());
		glEnd();
	}

	glTranslatef(current->offset(0), current->offset(1), current->offset(2));

	float w = max((float)-1, min((float)1, current->quaternion.w()));
	float angle = 2*acos(w)*RAD2DEG_SKE;
	glRotatef(angle, current->quaternion.x(), current->quaternion.y(), 
	          current->quaternion.z());

	for (unsigned int c = 0; c < current->children.size(); c++)
		glDrawSubTree(current->children[c]);

	glPopMatrix();
}


void skeleton::pose() // null pose
{
	for (unsigned int j = 0; j < channels.size(); j++) *channels[j] = 0.0;
	poseSubTree(root, 0.0);
}


void skeleton::pose(motion mot, double loopFrac, bool interpolation)
{
	unsigned int frame1, frame2;
	double timeIdx = loopFrac * (double)mot.sequence.size();
	float blend = 1.0;

	// load channel data corresponding to current time point
	if (interpolation) {
		frame1 = (unsigned int)floor(timeIdx);
		frame1 = min(frame1, (unsigned int)mot.sequence.size() - 1);
		frame2 = (unsigned int)ceil(timeIdx);
		frame2 = min(frame2, (unsigned int)mot.sequence.size() - 1);
		blend = (frame2 > frame1) ?  (float)timeIdx - (float)frame1 : 1.0;
		for (unsigned int j = 0; j < channels.size(); j++) {
			*channels[j] = mot.sequence[frame1][j];
			*channelsB[j] = mot.sequence[frame2][j];
		}
	} else { // if no interpolation, just use closest frame
		frame1 = (unsigned int)round(timeIdx);
		frame1 = min(frame1, (unsigned int)mot.sequence.size() - 1);
		for (unsigned int j = 0; j < channels.size(); j++)
			*channels[j] = mot.sequence[frame1][j];
		blend = 0.0;
	}
	// recursively pose
	poseSubTree(root, blend);
}


void skeleton::poseSubTree(joint* curr, float blend)
{
	curr->quaternion = quaternionFromEulers(curr->rotate, curr->channelNames);

	if (blend > 0.0) {
		if (curr->type == rootJoint)
			curr->transl = (1.0 - blend)*curr->transl + blend*curr->translB;
		curr->quaternionB = quaternionFromEulers(curr->rotateB, curr->channelNames);
		curr->quaternion = curr->quaternion.slerp(blend, curr->quaternionB);
	}
	for (unsigned int c = 0; c < curr->children.size(); c++)
		poseSubTree(curr->children[c], blend);
}


Quaternionf skeleton::quaternionFromEulers(Vector3f rotations, vector<string> axisNames)
{
	Vector4f newRotation;
	float angle;
	Quaternionf quaternion = Quaternionf::Identity();

	for (unsigned int n = 0; n < axisNames.size(); n++) {
		if (axisNames[n] == "Xrotation") {
			angle = rotations(0)*DEG2RAD_SKE/2.0;
			newRotation << Vector3f::UnitX() * sin(angle), cos(angle);
			quaternion *= (Quaternionf)newRotation;
		} else if (axisNames[n] == "Yrotation") {
			angle = rotations(1)*DEG2RAD_SKE/2.0;
			newRotation << Vector3f::UnitY() * sin(angle), cos(angle);
			quaternion *= (Quaternionf)newRotation;
		} else if (axisNames[n] == "Zrotation") {
			angle = rotations(2)*DEG2RAD_SKE/2.0;
			newRotation << Vector3f::UnitZ() * sin(angle), cos(angle);
			quaternion *= (Quaternionf)newRotation;
		}
	}
	return quaternion;
}


void skeleton::recoverBones()
{
	if (!vertices.size()) recoverSubTree(root, Vector3f::Zero(), -1);
}


void skeleton::recoverSubTree(joint* curr, Vector3f origin, int parent)
{
	static int count(-1);

	Vector3f vertex = origin + curr->offset;
	vertices.push_back(vertex);
	count++;
	int id(count);

	if (curr->type != rootJoint) {
		Vector2i bone(parent, id);
		bones.push_back(bone);
	}
	for (int c = 0; c < curr->children.size(); c++)
		recoverSubTree(curr->children[c], vertex, id);
}


void skeleton::glDrawBones(int highlightBone)
{
	for (int j = 0; j < bones.size(); j++) {
		int v0 = bones[j][0];
		int v1 = bones[j][1];
		if (j == highlightBone || highlightBone < 0)
			glColor3f(1.0, 0.0, 0.0);
		else
			glColor3f(0.1, 0.0, 0.0);
		glBegin(GL_LINES);
			glVertex3fv(vertices[v0].data());
			glVertex3fv(vertices[v1].data());
		glEnd();
	}
}


void skeleton::readBvhSkeleton(char* fileName)
{
	string line, token, name;
	ifstream infile(fileName);
	if (!infile) {
		cerr << "Error: unable to open input file: " << fileName << endl;
		exit(1);
	}
	seekLine(infile, "HIERARCHY"); // scan for HIERARCHY line

	if ((line = checkNextLine(infile, "ROOT")) != "") { // parse ROOT line
		istringstream stin(line);
		stin >> token >> name;
		root = acquireSubTree(infile, name, rootJoint);
		infile.close();
	} else {
		cerr << "Error: failed to read joint hierarchy" << endl;
		exit(1);
	}
}


joint* skeleton::acquireSubTree(ifstream& infile, string name, jointType type)
{
	string line, token;
	joint* current = new joint(name, type);

	checkNextLine(infile, "{"); // parse "{" line

	if ((line = checkNextLine(infile, "OFFSET")) != "") { // parse OFFSET line
		istringstream stin(line);
		stin >> token >> current->offset(0) >> current->offset(1) 
				 >> current->offset(2);
	}

	if (current->type == endSite) { // parse "}" line
		checkNextLine(infile, "}"); 
		return current;
	}

	if ((line = checkNextLine(infile, "CHANNELS")) != "") { // parse CHANNELS line
		istringstream stin(line);
		int num;
		stin >> token >> num;

		for (unsigned int i = 0; i < num; i++) {
			string channel;
			stin >> channel;
			if (channel == "Xrotation") {
				current->channelNames.push_back(channel);
				channels.push_back(&current->rotate(0));
				channelsB.push_back(&current->rotateB(0));
			} else if (channel == "Yrotation") {
				current->channelNames.push_back(channel);
				channels.push_back(&current->rotate(1));
				channelsB.push_back(&current->rotateB(1));
			} else if (channel == "Zrotation") {
				current->channelNames.push_back(channel);
				channels.push_back(&current->rotate(2));
				channelsB.push_back(&current->rotateB(2));
			} else if (channel == "Xposition") {
				if (current->type != rootJoint) {
					cerr << "Error: Xposition channel only allowed in ROOT" << endl; 
					exit(1);
				}
				current->channelNames.push_back(channel);
				channels.push_back(&current->transl(0));
				channelsB.push_back(&current->translB(0));
				translationIndices(0) = channels.size() - 1;
			} else if (channel == "Yposition") {
				if (current->type != rootJoint) {
					cerr << "Error: Yposition channel only allowed in ROOT" << endl; 
					exit(1);
				}
				current->channelNames.push_back(channel);
				channels.push_back(&current->transl(1));
				channelsB.push_back(&current->translB(1));
				translationIndices(1) = channels.size() - 1;
			} else if (channel == "Zposition") {
				if (current->type != rootJoint) {
					cerr << "Error: Zposition channel only allowed in ROOT" << endl; 
					exit(1);
				}
				current->channelNames.push_back(channel);
				channels.push_back(&current->transl(2));
				channelsB.push_back(&current->translB(2));
				translationIndices(2) = channels.size() - 1;
			} else {
				cerr << "Error: unrecognized channel type: " << channel << endl;
				exit(1);
			}
		}
	}

	while (getline(infile, line)) { // recursively parse children
		istringstream stin(line);
		stin >> token;
		string childName;
		if (token == "JOINT") {
			stin >> childName;
			joint* subTree = acquireSubTree(infile, childName, regularJoint);
			current->children.push_back(subTree);
		} else if (token == "End") {
			joint* subTree = acquireSubTree(infile, "End Site", endSite);
			current->children.push_back(subTree);
		} else if (token == "}") {
			break;
		} else {
			cerr << "Error: unrecognized line type starting with: " << token << endl;
			exit(1);
		}
	}

	if (token != "}") {
		cerr << "Error: missing '}'" << endl;
		exit(1);
	}
	return current;
}


float skeleton::maxBranchLength(joint* current)
{
	float maxChildNorm = 0.0;
	for (unsigned int c = 0; c < current->children.size(); c++)
		maxChildNorm = max(maxChildNorm, maxBranchLength(current->children[c]));
	return current->offset.norm() + maxChildNorm;
}


void skeleton::writeBvhSkeleton(char* fileName)
{
	ofstream outfile(fileName);
	if (!outfile) {
		cerr << "Error: unable to open output file: " << fileName << endl;
		exit(1);
	}
	outfile << "HIERARCHY" << endl;
	outputSubTree(outfile, root, 0);
	outfile.close();
}


void skeleton::outputSubTree(ofstream& outfile, joint* current, 
	                           unsigned int depth)
{
	string indent(2*depth,' ');
	string indent2(2*(depth+1),' ');

	switch (current->type) {
		case rootJoint:
			outfile << indent << "ROOT " << current->name << endl;
			break;
		case regularJoint:
			outfile << indent << "JOINT " << current->name << endl;
			break;
		case endSite:
			outfile << indent << current->name << endl;
			break;
		default:
			break;
	}
	outfile << indent << "{" << endl;
	outfile << indent2 << "OFFSET";
	for (unsigned int i = 0; i < current->offset.size(); i++) 
		outfile << " " << current->offset(i);
	outfile << endl;

	if (current->type != endSite) {
		outfile << indent2 << "CHANNELS " << current->channelNames.size();
		for (unsigned int i = 0; i < current->channelNames.size(); i++) 
			outfile << " " << current->channelNames[i];
		outfile << endl;
	}

	for (unsigned int c = 0; c < current->children.size(); c++)
		outputSubTree(outfile, current->children[c], depth+1);

	outfile << indent << "}" << endl;
}

