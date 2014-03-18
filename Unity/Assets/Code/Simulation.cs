using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

public class Simulation
{
	private const int FPS = 30;

	private static List<Agent> agents;

	public static void Start()
	{
		Application.targetFrameRate = FPS;
	}

	// Updates everything to the next frame!
	public static void Update()
	{
		if (Input.anyKeyDown) {
			Agent agent = AgentSpawner.GetAgent();

			// Finds a random entrance tile to spawn on
			// TODO: Only spawn in empty tiles
			var entrances = Grid.Instance.GetEntranceTiles();
			var entrance = entrances.ToArray()[Random.Range(0, entrances.Count)];
	
			agent.spawn(entrance.Position, Vector3.right);
		}
	}
}
