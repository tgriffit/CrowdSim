using UnityEngine;
using System.Collections.Generic;
using System.Linq;
using System;

namespace Simulation
{
	public class Agent
	{
		private float speed;								// Ideal walking speed in m/s
		private float speedPerFrame;						// How far each agent should move per frame
		private float speedMultiplier;						// How fast the agent is compared to the "ideal" speed

		// The prefab we should spawn, and its animator controller
		private GameObject model;
		public Animator anim;

		// our spawned instance
		private GameObject agent;
		private List<IPathAction<Tile>> path;

		// Each agent gets a unique color to depict their path so we can tell which path belongs to each one
		private Color debugColor;

		// Where the agent wants to end up
		public Tile Goal { get; set; }

		public Agent (GameObject m)
		{
			// An agent's speed is within 5% of 1.33 m/s
			speedMultiplier = UnityEngine.Random.Range(0.95f, 1.05f);
			speed = 1.33f * speedMultiplier;
			speedPerFrame = speed / Simulation.FPS;

			this.model = m;

			// Set up the animator
			this.anim = m.GetComponent<Animator>();

			// Creates a unique colour to display the path for each agent
			if (Simulation.Debug)
			{
				debugColor = new Color(UnityEngine.Random.Range(0.0f, 1.0f), UnityEngine.Random.Range(0.0f, 1.0f), UnityEngine.Random.Range(0.0f, 1.0f));
			}
		}

		// Spawns the agent facing towards it's goal tile.
		public bool Spawn(Tile location)
		{
			path = AStarHelper.Calculate(location, Goal, speedPerFrame);

			if (path == null 
				// TODO: Fix the issue with pathfinding that makes this necessary (without removing the occasional non-optimal paths if possible)
				|| path.OfType<PathfindingMovement>().Sum(pm => Vector3.Distance(pm.Origin.Position, pm.Destination.Position)) > 4*Vector3.Distance(location.Position, Goal.Position)
				|| path.OfType<PathfindingDelay>().Any(pd => pd.Delay > 50))
			{
				//Debug.LogError(String.Format("Failed to find path from [{0},{1}] to [{2},{3}]!", location.X, location.Z, Goal.X, Goal.Z));
				//Debug.DrawLine(location.Position, Goal.Position, Color.red, 5);
				return false;
			}

			agent = (GameObject)GameObject.Instantiate(model, location.Position, Quaternion.LookRotation(Goal.Position));
			ClaimPath();

			return true;
		}

		public void Unspawn()
		{
			Simulation.Instance.RemoveAgent(this);
			GameObject.Destroy(agent);

			// We could also walk through the agent's path and remove future tile claims if we wanted
		}

		public void Update()
		{
			DrawPath();

			if (path.Any())
			{
				Move();
			}
		}

		private void Move()
		{
			PathfindingMovement target = path.FirstOrDefault() as PathfindingMovement;
			PathfindingDelay delay = path.FirstOrDefault() as PathfindingDelay;

			if (target != null)
			{
				Tile destination = target.Destination;

				// Moves the agent towards the next tile in it's magical journey.
				agent.transform.position = Vector3.MoveTowards(agent.transform.position, target.Destination.Position, speedPerFrame);

				if (Vector3.Distance(destination.Position, agent.transform.position) < Simulation.Tolerance)
				{
					path.RemoveAt(0);

					if (path.Any())
					{
						// Rotate towards our new target
						var nextTarget = path.OfType<PathfindingMovement>().FirstOrDefault();

						if (nextTarget != null)
						{
							agent.transform.LookAt(nextTarget.Destination.Position);
						}
					}
					else
					{
						// We've reached our goal! Now we need to figure out what we're supposed to do now.
						var action = destination.GetTileAction();
				
						// TODO: If we add any actions that don't unspawn the agent, we need to give it a new goal.
						if (action != null)
						{
							action(this);
						}
					}
				}
			}
			else if (delay != null)
			{
				//Debug.Log ("Delay is occuring!: " + delay.Delay + " speed: " + model.GetComponent<Animator>().GetInteger("Speed".GetHashCode()));
				delay.Delay -= 1;

				// if waiting on a tile, play the idle animation
				if (delay.Delay > 5)
				{
					//anim.SetTrigger("StopWalking");
					Debug.Log ("To idle state");
					anim.Play ("Idle");
				}
				else if (delay.Delay <= 0)
				{
					path.RemoveAt(0);
					// now that the delay is over, start walking animations
					//anim.SetTrigger("StartWalking");
					Debug.Log ("To walking state");
					anim.Play ("Walking");
				}
			}
		}

		private void ClaimPath()
		{
			// The number of frames until we reach a given tile
			int time = 0;

			for (int i = 0; i < path.Count; ++i)
			{
				IPathAction<Tile> current = path.ElementAt(i);

				if (current is PathfindingDelay)
				{
					PathfindingDelay delay = current as PathfindingDelay;
					delay.Origin.AddClaim(time, delay.Delay, this, null);
					time += delay.Delay;
				}
				else
				{
					PathfindingMovement move = current as PathfindingMovement;
					float dist = Vector3.Distance(move.Origin.Position, move.Destination.Position);

					int numframes = (int)(dist / speedPerFrame);
					int halfframes = numframes / 2;
				
					// Half of the trip will be in each tile
					move.Origin.AddClaim(time, halfframes, this, move.Destination);
					move.Destination.AddClaim(time + halfframes, halfframes, this, null);

					time += numframes;
				}
			}
		}

		private void DrawPath()
		{
			if (Simulation.Debug)
			{
				if (!path.Any())
				{
					return;
				}

				foreach (IPathAction<Tile> action in path)
				{
					PathfindingMovement move = action as PathfindingMovement;
					PathfindingDelay delay = action as PathfindingDelay;

					if (move != null)
					{
						Debug.DrawLine(move.Origin.Position, move.Destination.Position, debugColor);
					}
					else
					{
						Vector3 topLeft = delay.Origin.Position + new Vector3(-Tile.TileSize / 4, 0, Tile.TileSize / 4);
						Vector3 topRight = delay.Origin.Position + new Vector3(Tile.TileSize / 4, 0, Tile.TileSize / 4);
						Vector3 bottomLeft = delay.Origin.Position + new Vector3(-Tile.TileSize / 4, 0, -Tile.TileSize / 4);
						Vector3 bottomRight = delay.Origin.Position + new Vector3(Tile.TileSize / 4, 0, -Tile.TileSize / 4);

						Debug.DrawLine(topLeft, bottomRight, debugColor);
						Debug.DrawLine(topRight, bottomLeft, debugColor);
					}
				}
			}
		}
	}
}
