using UnityEngine;
using System.Collections.Generic;
using System.Linq;

public class Agent
{
	private const float AGENTSPEED = 1.33f;								// Ideal walking speed in m/s
	private const float SPEEDPERFRAME = AGENTSPEED / Simulation.FPS;	// How far each agent should move per frame

	// The prefab we should spawn
	private GameObject model;

	// our spawned instance
	private GameObject agent;
	private List<Tile> path;

	// Each agent gets a unique color to depict their path so we can tell which path belongs to each one
	private Color debugColor;

	// Where the agent wants to end up
	public Tile Goal { get; set; }

	public Agent (GameObject m)
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
		agent = (GameObject)GameObject.Instantiate(model, location.Position, Quaternion.LookRotation(direction));

		path = AStarHelper.Calculate(location, Goal);

		// We don't need our current location in the path
		path.Remove(location);
	}

	public void Unspawn()
	{
		Simulation.Instance.RemoveAgent(this);
		GameObject.Destroy(agent);
	}

	public void Update()
	{
		var p = path.ToArray();
		Tile target = p[0];

		Debug.DrawLine(agent.transform.position, target.Position, debugColor);

		for (int i = 0; i < path.Count - 1; ++i)
		{
			Debug.DrawLine(p[i].Position, p[i + 1].Position, debugColor);
		}

		// Moves the agent towards the next tile in it's magical journey.
		agent.transform.position = Vector3.MoveTowards(agent.transform.position, target.Position, SPEEDPERFRAME);

		if (Vector3.Distance(target.Position, agent.transform.position) < 0.05)
		{
			path.RemoveAt(0);

			if (target == Goal)
			{
				// We've reached our goal! Now we need to figure out what we're supposed to do now.
				var action = target.GetTileAction();

				// TODO: If we add any actions that don't unspawn the agent, we need to give it a new goal.
				if (action != null)
				{
					action(this);
				}
			}
			else
			{
				// Rotate towards our new target
				var nextTarget = path.FirstOrDefault();
				agent.transform.LookAt(nextTarget.Position);
			}
		}
	}
}

