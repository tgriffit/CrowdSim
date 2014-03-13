using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class Grid
{
	// Singleton instance of the grid
	public static Grid Instance = new Grid();
	private Tile[][] grid;

	private const float TOLERANCE = 0.05f;		// I have no idea what our tolerance for y-variation should be

	private bool setupDone = false;
	public void setup(Vector3 corner, int x, int z, Vector3[] vertices, int[] triangles)
	{
		if (setupDone)
		{
			Debug.LogError("Tried to run setup on the grid multiple times!");
			return;
		}

		// A list of all the vertices that are part of an obstacle
		var problemVertices = vertices.Where(v => Mathf.Abs(v.y - corner.y) > TOLERANCE);
		// All triangles that contain an obstacle vertex
		var problemPolies = triangles.Where(t => t%3==0 && problemVertices.Contains(vertices[t]) 
		                                    || problemVertices.Contains(vertices[t+1]) 
		                                    || problemVertices.Contains(vertices[t+2]));

		grid = new Tile[x][];
		for (int i = 0; i < x; ++i)
		{
			grid[i] = new Tile[z];

			for (int j = 0; j < grid[i].Length; ++j)
			{
				var tileCorner = new Vector3(corner.x + i * Tile.TILESIZE, corner.y, corner.z + j * Tile.TILESIZE);

				grid[i][j] = new Tile(tileCorner);
			}
		}

		// Walk through the grid and set up the tile properties
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
