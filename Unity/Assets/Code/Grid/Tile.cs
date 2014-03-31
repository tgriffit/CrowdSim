using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEditor;

public class Tile : IPathNode<Tile>
{
	public const float TILESIZE = 3.0f;		// Should change to match the width of our model

	private const float TOLERANCE = 0.05f;		// I have no idea what our tolerance for y-variation should be
	private const float CLEARANCE = 3.0f;		// The minimum clearance over the ground for a tile to be considered IsWalkable

	public Tile (Vector3 corner, int x, int z)
	{
		X = x;
		Z = z;

		Position = new Vector3(corner.x + TILESIZE / 2, corner.y, corner.z + TILESIZE / 2);

		IsWalkable = TestWalkability();
		claims = new List<TileClaim>();
	}

	// Basic Tile properties
	public bool IsWalkable { get; set; }

	public bool IsEntrance { get; set; }

	public bool IsExit { get; set; }

	#region IPathNode members
	public bool Invalid { get { return !IsWalkable; } }
	public List<Tile> Connections { get; set; }

	public float GetDistanceHeuristic(Tile other)
	{
		// Finds the number of orthogonal tile movements that would
		// be required to pass between the two tiles without taking
		// obstacles into account.
		return Math.Abs(other.X - X) + Math.Abs(other.Z - Z);
	}

	public float GetDistance(Tile other, int framenum, float speed)
	{
		float dist = Vector3.Distance(Position, other.Position);
		int framesOfMovement = (int)(dist / speed);

		return framesOfMovement + other.FindDelay(framenum + framesOfMovement/2, framesOfMovement);
	}

	public List<IPathAction<Tile>> GetPathTo(Tile other, float speed, ref int time)
	{
		List<IPathAction<Tile>> list = new List<IPathAction<Tile>>();

		float dist = Vector3.Distance(Position, other.Position);
		int framesOfMovement = (int)(dist / speed);
		//int delay = other.FindDelay(time, framesOfMovement);
		int delay = other.FindDelay(time + framesOfMovement / 2, framesOfMovement);

//		if (other.X == 3 && other.Z == 1)
//		{
//			Debug.Log(String.Format("[{0},{1}] time: {2} {3} {4}", X, Z, time, delay, otherDelay));
//		}
		
		//delay += otherDelay;

		if (delay > 0)
		{
//			Debug.Log(other.X + " " + other.Z);
			list.Add(new PathfindingDelay() { Origin = this, Delay = delay });
		}

		//list.Add (new PathfindingDelay () {Origin = this, Delay = 10});
		list.Add(new PathfindingMovement() { Origin = this, Destination = other });

		time += framesOfMovement + delay;
		return list;
	}
	#endregion

	public void Update()
	{
		claims.ForEach(c => { --c.StartTime; --c.EndTime; });
		claims.RemoveAll(c => c.EndTime <= 0);
	}

	// The center of the tile
	public Vector3 Position { get; private set; }

	// The tile's position in the grid
	public int X { get; private set; }
	public int Z { get; private set; }	// We'll continue using Z instead of Y to remain consistent.

	// Agents can "claim" the tile a certain number of updates in advance.
	// We keep track of when each one will be in the tile so that others can
	// move through it up until that point.
	private List<TileClaim> claims;
	public void AddClaim(int start, int duration)
	{
		//int d = start + duration;
		//Debug.Log(String.Format("[{0},{1}]: ({2},{3})", X, Z, start, d));
		claims.Add(new TileClaim(){ StartTime = start - 3, EndTime = start+duration });
		claims.OrderBy(c => c.StartTime);
	}

	public bool CurrentlyClaimed()
	{
		var first = claims.FirstOrDefault();

		// If the start time of the first element is greater than 0 then that
		// claim hasn't started yet. Since we constantly sort the claims, the
		// first one is guaranteed to have the lowest start time.
		return !(first == null || first.StartTime > 0);
	}

	public bool MultipleClaims()
	{
		return (claims.Count(c => c.StartTime <= 0) > 1);
	}

	// The action to take when an agent reaches it's goal.
	// TODO: Add more action types to make the simulation more realistic. This 
	// may have to wait until after the project is over.
	public Action<Agent> GetTileAction()
	{
		if (IsExit)
		{
			return new Action<Agent>(a => a.Unspawn());
		}

		return null;
	}

	private bool TestWalkability()
	{
		// Throws a sphere at the terrain to detect obstacles.
		// This should also detect any static objects that get added to the scene, allowing users to
		// spruce up the environment.
		return !Physics.SphereCastAll(Position - new Vector3(0, TILESIZE, 0), TILESIZE / 2, Vector3.up).Any();
	}

	// Finds how long an agent will have to delay if it wants to enter this tile at a time starting at
	// time and entering the tile will take duration time.
	private int FindDelay(int time, int duration)
	{
		// The number of frames the agent will have to wait before it
		// can enter the tile.
		int delay = 0;

		// GAH. Algorithms are hard.
		foreach (TileClaim claim in claims)
		{
			// Check to see whether our journey will interfere with an existing claim
			if ((claim.StartTime <= time + delay && claim.EndTime >= time + delay)
			    || (claim.StartTime <= time + duration + delay && claim.EndTime >= time + duration + delay)
			    || (claim.StartTime >= time + delay && claim.EndTime <= time + duration + delay))
			{
				delay += claim.EndTime - time;
			}
		}

		return delay;
	}

	public void DebugDraw()
	{
		if (Simulation.Debug)
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

			if (!IsWalkable)
			{
				Debug.DrawLine(upperLeft, lowerRight, Color.red);
				Debug.DrawLine(upperRight, lowerLeft, Color.red);
			}

			if (IsEntrance)
			{
				Debug.DrawLine(upperMid, lowerMid, Color.blue);
				Debug.DrawLine(leftMid, rightMid, Color.blue);
			}

			if (IsExit)
			{
				Debug.DrawLine(upperMid, rightMid, Color.blue);
				Debug.DrawLine(rightMid, lowerMid, Color.blue);
				Debug.DrawLine(lowerMid, leftMid, Color.blue);
				Debug.DrawLine(leftMid, upperMid, Color.blue);
			}

			if (MultipleClaims())
			{
				Debug.DrawLine(upperLeft, upperMid, Color.red);
				Debug.DrawLine(upperRight, upperMid, Color.red);
				Debug.DrawLine(lowerLeft, lowerMid, Color.red);
				Debug.DrawLine(lowerRight, lowerMid, Color.red);
				Debug.DrawLine(upperLeft, leftMid, Color.red);
				Debug.DrawLine(lowerLeft, leftMid, Color.red);
				Debug.DrawLine(upperRight, rightMid, Color.red);
				Debug.DrawLine(lowerRight, rightMid, Color.red);
			}
			else if (CurrentlyClaimed())
			{
				Debug.DrawLine(upperLeft, upperMid, Color.blue);
				Debug.DrawLine(upperRight, upperMid, Color.blue);
				Debug.DrawLine(lowerLeft, lowerMid, Color.blue);
				Debug.DrawLine(lowerRight, lowerMid, Color.blue);
				Debug.DrawLine(upperLeft, leftMid, Color.blue);
				Debug.DrawLine(lowerLeft, leftMid, Color.blue);
				Debug.DrawLine(upperRight, rightMid, Color.blue);
				Debug.DrawLine(lowerRight, rightMid, Color.blue);
			}
		}
	}
}