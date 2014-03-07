#include "Rig.h"
#include "FileParser.h"


void Rig::readRigFile(string filename)
{
	string line;

	ifstream file(filename);
	if (!file)
	{
		error("ERROR: Unable to open rig file: %s\n", filename);
	}

	seekLine(file, "MESH");
	if (getline(file, line))
	{
		obj.readObjFile((directoryFromPath(filename) + line).c_str());
	}
	
	// TODO: Read the rest of the file (once we figure out the format)
}