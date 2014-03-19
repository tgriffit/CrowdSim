using UnityEngine;
using System.Collections.Generic;

public class Agent
{
	// The prefab we should spawn
	private Transform model;
	private List<Tile> path;

	// Each agent gets a unique color to depict their path so we can tell which path belongs to each one
	private Color debugColor;

	// Where the agent wants to end up
	public Tile Goal { get; set; }

	public Agent (Transform m)
	{
		this.model = m;
		debugColor = new Color(Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f));
	}

	// Spawns the agent facing towards it's goal tile.
	public void Spawn(Tile location)
	{
		Spawn(location, Goal.Position);
	}

	// Spawns the agent facing towards the specified location.
	public void Spawn(Tile location, Vector3 direction)
	{
		UnityEngine.Object.Instantiate(model, location.Position, Quaternion.LookRotation(direction));

		path = AStarHelper.Calculate(location, Goal);
		Debug.Log(path.Contains(location));
	}

	public void Update()
	{
		var p = path.ToArray();

		for (int i = 0; i < path.Count - 1; ++i)
		{
			Debug.DrawLine(p[i].Position, p[i+1].Position, debugColor);
		}
	}
}

