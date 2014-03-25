using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class Grid
{
	// Singleton instance of the grid
	//public static Grid Instance = new Grid();
	private Tile[][] grid;

	private int maxX;
	private int maxZ;

	public Grid(Vector3 corner, int x, int z, Vector3[] vertices, int[] triangles)
	{	
		maxX = x;
		maxZ = z;

		grid = new Tile[x][];
		for (int i = 0; i < x; ++i)
		{
			grid[i] = new Tile[z];

			for (int j = 0; j < grid[i].Length; ++j)
			{
				grid[i][j] = new Tile(new Vector3(corner.x + i * Tile.TILESIZE, corner.y, corner.z + j * Tile.TILESIZE), i, j);
			}
		}

		// Once we're done creating the tiles, we can figure out what's adjacent to what
		GetAllTiles().ForEach(t => t.Connections = GetAdjacentTiles(t).ToList());

		// Locate all of the ways in and out of the environment
		FindEntrancesAndExits();
	}

	public void Update()
	{
		GetAllTiles().ForEach(t => t.Update());
	}

	public List<Tile> GetAllTiles()
	{
		return grid.SelectMany(t => t).ToList();
	}

	public List<Tile> GetEntranceTiles()
	{
		return GetAllTiles().Where(t => t.IsEntrance && t.IsWalkable && !t.CurrentlyClaimed()).ToList();
	}

	public List<Tile> GetExitTiles()
	{
		return GetAllTiles().Where(t => t.IsExit && t.IsWalkable).ToList();
	}

	public IEnumerable<Tile> GetAdjacentTiles(Tile t)
	{
		for (int i = Math.Max(t.X - 1, 0); i < Math.Min(t.X + 2, maxX); ++i)
		{
			for (int j = Math.Max(t.Z - 1, 0); j < Math.Min(t.Z + 2, maxZ); ++j)
			{
				if (!(i == t.X && j == t.Z))
				{
					yield return grid[i][j];
				}
			}
		}
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
		int minx = (int)Math.Max(tile.X - p.xsize, 0);
		int maxx = (int)Math.Min(tile.X + p.xsize, grid.Length - 1);

		int minz = (int)Math.Max(tile.Z - p.zsize, 0);
		int maxz = (int)Math.Min(tile.Z + p.zsize, grid[grid.Length - 1].Length);

		// Returns all the tiles within the appropriate range of the center tile
		return tiles.Where(t => t.X >= minx && t.X <= maxx && t.Z >= minz && t.Z <= maxz).ToList();
	}


	public void DebugDraw()
	{
		GetAllTiles().ForEach(t => t.DebugDraw());
	}
}
