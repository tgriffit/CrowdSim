#include <stdio.h>
#include <iostream>

#include "Utilities.h"

int main(int argc, char* argv[])
{
	string env = "";
	string rig = "";
	int pop = 0;

	// Read the arguments to figure out what the user has in store for us today.
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--environment") == 0)
		{
			// Read the 3D environment's .obj file
			env = argv[++i];
		}
		else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--rig") == 0)
		{
			// Read the animation rig (which includes .bvh files and a .obj file)
			rig = argv[++i];
		}
		else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--population") == 0)
		{
			// Sets the intial population max of the simulation
			pop = atoi(argv[++i]);
		}
		else
		{
			cout << "Couldn't understand the command line option: " << argv[i] << endl;
			cout << "Exiting..." << endl;
			exit(0);
		}
	}

	if (env.empty())
	{
		error("No environment file specified! You need to specify one with the -e flag");
	}
	else if (rig.empty())
	{
		error("No rig file specified! You need to specify one with the -r flag");
	}
	else
	{
		// Run the actual program
	}

	return 0;
}