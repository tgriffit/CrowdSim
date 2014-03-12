using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class Grid
{
	// Singleton instance of the grid
	public static Grid Instance = new Grid();
	private Tile[][] grid;


	private bool setupDone = false;
	public void setup(Vector3 corner, int x, int z)
	{
		if (setupDone)
		{
			Debug.LogError("Tried to run setup on the grid multiple times!");
			return;
		}

		grid = new Tile[x][];
		for (int i = 0; i < x; ++i)
		{
			grid[i] = new Tile[z];

			for (int j = 0; j < grid[i].Length; ++j)
			{
				grid[i][j] = new Tile(new Vector3(corner.x + i * Tile.TILESIZE, corner.y, corner.z + j * Tile.TILESIZE));
			}
		}
	}

	public List<Tile> getAllTiles()
	{
		List<Tile> tiles = new List<Tile>();

		foreach (Tile[] ts in grid)
		{
			tiles.AddRange(ts);
		}

		return tiles;
	}

	public void DebugDraw()
	{
		getAllTiles().ForEach(t => t.DebugDraw());
	}
}
