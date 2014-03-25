using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

public class Simulation
{
	public static Simulation Instance = new Simulation();

	public const bool Debug = true;

	public const int FPS = 30;								// Our desired frames per second

	private Grid grid;
	private List<Agent> agents;

	private int maxpopulation;
	public int MaxPopulation 
	{ 
		get { return maxpopulation; } 
		set
		{
			maxpopulation = value < 0 ? 0 : value;
		}
	}

	// The current number of agents in the simulation
	public int Population { get { return agents.Count; } }
	public bool Playing { get; set; }

	public void Start()
	{
		Application.targetFrameRate = FPS;
		agents = new List<Agent>();
		maxpopulation = 2;
		Playing = true;
	}

	// Updates everything to the next frame!
	public void Update()
	{
		grid.DebugDraw();
		
		if (Playing)
		{
			grid.Update ();

			agents.ForEach (a => a.Update ());

			if (agents.Count < maxpopulation)
			{
				AddAgent();
			} 
			else if (agents.Count > maxpopulation)
			{
				Agent a = agents.FirstOrDefault ();

				if (a != null)
				{
					a.Unspawn();
				}
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

		if (!entrances.Any())
		{
			// No proper entrances found. We'll have to wait until one frees up.
			return;
		}

		var entrance = Utilities.GetRandomEntry(entrances);

		agent.Goal = FindGoal(entrance);
		
		if (agent.Spawn(entrance))
		{
			agents.Add(agent);
		}
	}

	public void RemoveAgent(Agent a)
	{
		agents.Remove(a);
	}
	
	// Finds a random exit tile. We may want to add some other criteria as well.
	// We also 
	private Tile FindGoal(Tile entrance)
	{
		var exits = grid.GetExitTiles();
		return Utilities.GetRandomEntry(exits);
	}
}
