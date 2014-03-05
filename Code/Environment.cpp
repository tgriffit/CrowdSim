#include "Environment.h"
#include "FileParser.h"

void Environment::loadEnvironment(char* filename)
{
	string line;

	ifstream file(filename);
	if (!file)
	{
		error("ERROR: Unable to open environment file: %s", filename);
	}

	seekLine(file, "MODEL");
	if (getline(file, line))
	{
		readObjFile(line.c_str());
	}
	
	// TODO: Read the rest of the file (once we figure out the format)

	setupGrid();
}

void Environment::setupGrid()
{

}