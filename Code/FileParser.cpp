#include "FileParser.h"

string seekLine(ifstream& file, string pattern)
{
	string line, token;
	while (getline(file, line)) 
	{
		istringstream stin(line);
		stin >> token;
		if (token == pattern) return line;
	}

	error("ERROR: no line found that starts with: %s", pattern);
}


string checkNextLine(ifstream& file, string pattern)
{
	string line, token;
	if (getline(file, line)) 
	{
		istringstream stin(line);
		stin >> token;
		if (token == pattern) return line;
	}

	error("ERROR: line does not start with required token: %s", pattern);
}