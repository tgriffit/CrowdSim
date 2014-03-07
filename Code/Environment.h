#pragma once

#include "Model.h"

enum TileType 
{
	INVALID,	// Tile is not walkable
	PATH,		// Tile is a valid target for pathfinding, but isn't an entrance or exit
	ENTRANCE,
	EXIT		// May go away, or may add ENTRANCEEXIT state
};

struct Environment : public Model
{
	TileType** tiles;
	int length, width;	// Dimensions of the environment (in tiles)

	void loadEnvironment(string filename);

	void setupGrid();
};