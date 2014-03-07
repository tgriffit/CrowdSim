#include "Environment.h"
#include "FileParser.h"

void Environment::loadEnvironment(string filename)
{
	string line;

	ifstream file(filename);
	if (!file)
	{
		error("ERROR: Unable to open environment file: %s\n", filename);
	}

	seekLine(file, "MESH");
	if (getline(file, line))
	{
		readObjFile((directoryFromPath(filename) + line).c_str());
	}
	
	// TODO: Read the rest of the file (once we figure out the format)

	setupGrid();
}

void Environment::setupGrid()
{
	// Figure out the dimensions of the environment
	
	// Divide the environment into tiles

	// Allocate the tiles array

	// Mark tiles with their values (from the TileType enum)
}