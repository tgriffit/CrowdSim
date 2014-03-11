using System;
using System.Collections.Generic;
using UnityEngine;

public class Tile
{
	//public enum TileType { WALKABLE, UNWALKABLE }
	public const float TILESIZE = 1.0f;	// Should change
	private Vector3 position;

	public Tile (Vector3 corner)
	{
		Position = new Vector3(corner.x + TILESIZE / 2, corner.y, corner.z + TILESIZE / 2);
		Walkable = true;
	}

	// Basic Tile properties
	public bool Walkable { get; set; }
	public bool IsEntrance { get; set; }
	public bool IsExit { get; set; }

	// The center of the tile
	public Vector3 Position { get; private set; }

	// Agents can "claim" the tile a certain number of updates in advance.
	// We keep track of when each one will be in the tile so that others can
	// move through it up until that point.
	public List<int> Claims { get; set; }



	public void DebugDraw()
	{
		var upperLeft = new Vector3(Position.x - TILESIZE/2, Position.y, Position.z + TILESIZE/2);
		var upperRight = new Vector3(Position.x + TILESIZE/2, Position.y, Position.z + TILESIZE/2);
		var lowerLeft = new Vector3(Position.x - TILESIZE/2, Position.y, Position.z - TILESIZE/2);
		var lowerRight = new Vector3(Position.x + TILESIZE/2, Position.y, Position.z - TILESIZE/2);

		Debug.DrawLine(upperLeft, upperRight, Color.white);
		Debug.DrawLine(upperRight, lowerRight, Color.white);
		Debug.DrawLine(lowerRight, lowerLeft, Color.white);
		Debug.DrawLine(lowerLeft, upperLeft, Color.white);

		if (!Walkable)
		{
			Debug.DrawLine(upperLeft, lowerRight, Color.red);
			Debug.DrawLine(upperRight, lowerLeft, Color.red);
		}

		if (IsEntrance)
		{

		}

		if (IsExit)
		{

		}
	}
}