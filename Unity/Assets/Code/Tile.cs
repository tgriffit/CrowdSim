using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class Tile
{
	//public enum TileType { WALKABLE, UNWALKABLE }
	public const float TILESIZE = 0.75f;	// Should change

	private const float TOLERANCE = 0.05f;		// I have no idea what our tolerance for y-variation should be
	private const float CLEARANCE = 3.0f;		// The minimum clearance over the ground for a tile to be considered walkable

	//private Vector3 position;

	public Tile (Vector3 corner, int x, int z)
	{
		Position = new Vector3(corner.x + TILESIZE / 2, corner.y, corner.z + TILESIZE / 2);
		GridPosition = new Vector2(x, z);
		Walkable = TestWalkability();
	}

	// Basic Tile properties
	public bool Walkable { get; set; }

	public bool IsEntrance { get; set; }

	public bool IsExit { get; set; }

	// The center of the tile
	public Vector3 Position { get; private set; }

	// The tile's position in the grid
	public Vector2 GridPosition { get; private set; }

	// Agents can "claim" the tile a certain number of updates in advance.
	// We keep track of when each one will be in the tile so that others can
	// move through it up until that point.
	public List<int> Claims { get; set; }

	private bool TestWalkability()
	{
		// Throws a sphere at the terrain to detect obstacles.
		// This should also detect any static objects that get added to the scene, allowing users to
		// spruce up the environment.
		return !Physics.SphereCastAll(Position - new Vector3(0, TILESIZE, 0), TILESIZE / 2, Vector3.up).Any();
	}

	public void DebugDraw()
	{
		var upperLeft = new Vector3(Position.x - TILESIZE / 2, Position.y, Position.z + TILESIZE / 2);
		var upperRight = new Vector3(Position.x + TILESIZE / 2, Position.y, Position.z + TILESIZE / 2);
		var lowerLeft = new Vector3(Position.x - TILESIZE / 2, Position.y, Position.z - TILESIZE / 2);
		var lowerRight = new Vector3(Position.x + TILESIZE / 2, Position.y, Position.z - TILESIZE / 2);

		// These points are the midpoints between the center and the edges - if they
		// are on the edge lines it becomes too chaotic.
		var upperMid = new Vector3(Position.x, Position.y, Position.z + TILESIZE / 4);
		var lowerMid = new Vector3(Position.x, Position.y, Position.z - TILESIZE / 4);
		var leftMid = new Vector3(Position.x - TILESIZE / 4, Position.y, Position.z);
		var rightMid = new Vector3(Position.x + TILESIZE / 4, Position.y, Position.z);

		Debug.DrawLine(upperLeft, upperRight, Color.blue);
		Debug.DrawLine(upperRight, lowerRight, Color.blue);
		Debug.DrawLine(lowerRight, lowerLeft, Color.blue);
		Debug.DrawLine(lowerLeft, upperLeft, Color.blue);

		if (!Walkable)
		{
			Debug.DrawLine(upperLeft, lowerRight, Color.red);
			Debug.DrawLine(upperRight, lowerLeft, Color.red);
		}

		if (IsEntrance)
		{
			Debug.DrawLine(upperMid, lowerMid, Color.cyan);
			Debug.DrawLine(leftMid, rightMid, Color.cyan);
		}

		if (IsExit)
		{
			Debug.DrawLine(upperMid, rightMid, Color.magenta);
			Debug.DrawLine(rightMid, lowerMid, Color.magenta);
			Debug.DrawLine(lowerMid, leftMid, Color.magenta);
			Debug.DrawLine(leftMid, upperMid, Color.magenta);
		}
	}
}