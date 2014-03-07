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

	error("ERROR: no line found that starts with: %s\n", pattern);
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

	error("ERROR: line does not start with required token: %s\n", pattern);
}

string directoryFromPath(string path)
{
	size_t pos = path.find_last_of("/\\");
	if (pos != string::npos)
	{
		return path.substr(0, pos+1);
	}
	else
	{
		return "";
	}
}