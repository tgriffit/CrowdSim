using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

public class Simulation
{
	public static Simulation Instance = new Simulation();


	private const int FPS = 30;								// Our desired frames per second
	private const float AGENTSPEED = 1.33f;					// Ideal walking speed in m/s
	private const float SPEEDPERFRAME = AGENTSPEED / FPS;	// How far each agent should move per frame

	private int population = 1;

	private Grid grid;
	private List<Agent> agents;

	public void Start()
	{
		Application.targetFrameRate = FPS;

		agents = new List<Agent>();
	}

	// Updates everything to the next frame!
	public void Update()
	{
		grid.DebugDraw();

		if (Input.GetKeyDown(KeyCode.KeypadPlus)) 
		{
			++population;
		}

		if (Input.GetKeyDown(KeyCode.KeypadMinus))
		{
			--population;
		}

		agents.ForEach(a => a.Update());

		if (agents.Count < population)
		{
			AddAgent();
		}
	}

	public void SetGrid(Grid g)
	{
		grid = g;
	}

	private void AddAgent()
	{
		Agent agent = AgentSpawner.GetAgent();
		
		// Finds a random entrance tile to spawn on
		// TODO: Only spawn in empty tiles
		var entrances = grid.GetEntranceTiles();
		var entrance = entrances.ToArray()[Random.Range(0, entrances.Count)];

		agent.Goal = FindGoal(entrance);
		
		agent.Spawn(entrance);
		agents.Add(agent);
	}
	
	// Finds a random exit tile. We may want to add some other criteria as well.
	private Tile FindGoal(Tile entrance)
	{
		var exits = grid.GetExitTiles().ToArray();
		return exits[Random.Range(0, exits.Length)];
	}
}
