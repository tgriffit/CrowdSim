using UnityEngine;

public class Agent
{
	// The prefab we should spawn
	private Transform model;

	// Where the agent wants to end up
	private Tile goal;

	public Agent (Transform m)
	{
		this.model = m;
		goal = FindGoal();
	}

	// Spawns the agent facing towards it's goal tile.
	public void spawn(Vector3 position)
	{
		spawn(position, goal.Position);
	}

	// Spawns the agent facing towards the specified location.
	public void spawn(Vector3 position, Vector3 direction)
	{
		UnityEngine.Object.Instantiate(model, position, Quaternion.LookRotation(direction));
	}

	// Finds a random exit tile. We may want to add some other criteria as well.
	private Tile FindGoal()
	{
		var exits = Grid.Instance.GetExitTiles().ToArray();
		return exits[Random.Range(0, exits.Length)];
	}
}

