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
	public void setup(Vector3 corner, int x, int z, Vector3[] vertices, int[] triangles)
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
				grid[i][j] = new Tile(new Vector3(corner.x + i * Tile.TILESIZE, corner.y, corner.z + j * Tile.TILESIZE), i, j);
			}
		}

		// Locate all of the ways in and out of the environment
		FindEntrancesAndExits();
	}

	public List<Tile> GetAllTiles()
	{
		return grid.SelectMany(t => t).ToList();
	}

	public List<Tile> GetEntranceTiles()
	{
		return GetAllTiles().Where(t => t.IsEntrance).ToList();
	}

	public List<Tile> GetExitTiles()
	{
		return GetAllTiles().Where(t => t.IsExit).ToList();
	}

	private void FindEntrancesAndExits()
	{
		var entrances = UnityEngine.Object.FindObjectsOfType<Entrance>();
		var exits = UnityEngine.Object.FindObjectsOfType<Exit>();
		var tiles = GetAllTiles();
		
		entrances.ToList().ForEach(e => GetNearTiles(e, tiles).ToList().ForEach(t => t.IsEntrance = true));
		exits.ToList().ForEach(e => GetNearTiles(e, tiles).ToList().ForEach(t => t.IsExit = true));
	}

	private List<Tile> GetNearTiles(InteractionPoint p, IEnumerable<Tile> tiles)
	{
		var shortestDist = tiles.Min(t => Vector3.Distance(p.Position, t.Position));
		var tile = tiles.First(t => Vector3.Distance(p.Position, t.Position) == shortestDist);

		// Once we have the nearest tile, we need to find that tile's neighbours
		var center = tile.GridPosition;

		int minx = (int)Math.Max(center.x - p.xsize, 0);
		int maxx = (int)Math.Min(center.x + p.xsize, grid.Length - 1);

		int minz = (int)Math.Max(center.y - p.zsize, 0);
		int maxz = (int)Math.Min(center.y + p.zsize, grid[grid.Length - 1].Length);

		// Returns all the tiles within the appropriate range of the center tile
		return tiles.Where(t => t.GridPosition.x >= minx && t.GridPosition.x <= maxx && t.GridPosition.y >= minz && t.GridPosition.y <= maxz).ToList();
	}


	public void DebugDraw()
	{
		GetAllTiles().ForEach(t => t.DebugDraw());
	}
}
