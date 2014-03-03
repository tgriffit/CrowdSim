
#include "skeleton.h"
#include "motion.h"

using namespace std;
using namespace Eigen;


void motion::readBvhMotion(char* fileName, skeleton skel)
{
	string line, token, token2;
	unsigned int numFrames;

	ifstream infile(fileName);
	if (!infile) {
		cerr << "Error: unable to open input file: " << fileName << endl;
		exit(1);
	}

	seekLine(infile, "MOTION"); // scan for MOTION line

	if ((line = checkNextLine(infile, "Frames:")) != "") { // parse Frames line
		istringstream stin(line);
		stin >> token >> numFrames;
	}

	if (getline(infile, line)) { // parse Frame Time line
		istringstream stin(line);
		stin >> token >> token2;
		if (token != "Frame" || token2 != "Time:") {
			cerr << "Error: missing Frame Time line" << endl;
			exit(1);
		}
		stin >> defaultGapTime;
	}

	for (unsigned int i = 0; i < numFrames; i++) { // read motion data
		if (!getline(infile, line)) {
			cerr << "Error: not enough motion lines to match frame count: " 
					 << numFrames << endl;
			exit(1);
		}
		istringstream stin(line);
		vector<float> channelData;
		float channelDatum;
		for (unsigned int j = 0; j < skel.channels.size(); j++) {
			if (!(stin >> channelDatum)) {
				cerr << "Error: not enough motion data in line " << i << 
				        " to match channel count: " << skel.channels.size() << endl;
				exit(1);
			}
			channelData.push_back(channelDatum);
		}
		sequence.push_back(channelData);
	}
}


vector<Vector3f> motion::motionBounds(Vector3i translationIndices)
{
	Vector3f mins(FLT_MAX, FLT_MAX, FLT_MAX);
	Vector3f maxs(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (unsigned int i = 1; i < sequence.size(); i++) { // ignore frame 0
		mins[0] = min(mins[0], sequence[i][translationIndices[0]]);
		mins[1] = min(mins[1], sequence[i][translationIndices[1]]);
		mins[2] = min(mins[2], sequence[i][translationIndices[2]]);
		maxs[0] = max(maxs[0], sequence[i][translationIndices[0]]);
		maxs[1] = max(maxs[1], sequence[i][translationIndices[1]]);
		maxs[2] = max(maxs[2], sequence[i][translationIndices[2]]);
	}
	Vector3f centers((mins + maxs)/2.0);
	Vector3f ranges(maxs - mins);
	vector<Vector3f> bounds;
	bounds.push_back(centers);
	bounds.push_back(ranges);
	return bounds;
}


void motion::writeBvhMotion(char* fileName)
{
	ofstream outfile(fileName, fstream::out | fstream::app);
	if (!outfile) {
		cerr << "Error: unable to open output file: " << fileName << endl;
		exit(1);
	}
	outfile << "MOTION" << endl;
	outfile << "Frames: " << sequence.size() << endl;
	outfile << "Frame Time: " << defaultGapTime << endl;
	for (unsigned int i = 0; i < sequence.size(); i++) {
		for (unsigned int j = 0; j < sequence[i].size(); j++) 
			outfile << sequence[i][j] << " ";
		outfile << endl;
	}
	outfile.close();
}

