
#include "bvh_util.h"

using namespace std;


string seekLine(ifstream& infile, string pattern)
{
	string line, token;
	while (getline(infile, line)) {
		istringstream stin(line);
		stin >> token;
		if (token == pattern) return line;
	}
	cerr << "Error: no line found that starts with: " << pattern << endl;
	exit(1);
}


string checkNextLine(ifstream& infile, string pattern)
{
	string line, token;
	if (getline(infile, line)) {
		istringstream stin(line);
		stin >> token;
		if (token == pattern) return line;
	}
	cerr << "Error: line does not start with required token: " << pattern << endl;
	exit(1);
}

