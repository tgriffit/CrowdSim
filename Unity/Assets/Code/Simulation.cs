using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

public class Simulation
{
	public static Simulation Instance = new Simulation();

	public const int FPS = 30;								// Our desired frames per second

	private Grid grid;
	private List<Agent> agents;

	private int maxpopulation = 1;

	// The current number of agents in the simulation
	public int Population { get { return agents.Count; } }

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
			ChangePopulation(1);
		}
		
		if (Input.GetKeyDown(KeyCode.KeypadMultiply))
		{
			ChangePopulation(10);
		}

		if (Input.GetKeyDown(KeyCode.KeypadMinus))
		{
			ChangePopulation(-1);
		}
		
		if (Input.GetKeyDown(KeyCode.KeypadDivide))
		{
			ChangePopulation(-10);
		}

		agents.ForEach(a => a.Update());

		if (agents.Count < maxpopulation)
		{
			AddAgent();
		}
		else if (agents.Count > maxpopulation)
		{
			Agent a = agents.FirstOrDefault();

			if (a != null)
			{
				a.Unspawn();
			}
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

	public void RemoveAgent(Agent a)
	{
		agents.Remove(a);
	}
	
	public void ChangePopulation(int mod)
	{
		maxpopulation += mod;
		maxpopulation = maxpopulation < 0 ? 0 : maxpopulation;
	}
	
	// Finds a random exit tile. We may want to add some other criteria as well.
	// We also 
	private Tile FindGoal(Tile entrance)
	{
		var exits = grid.GetExitTiles().ToArray();
		return exits[Random.Range(0, exits.Length)];
	}
}
