using System;

namespace Simulation
{
	public class TileClaim
	{
		// How many frames until the agent arrives in the tile
		public int StartTime { get; set; }

		// How many frames until the agent is no longer in the tile
		public int EndTime { get; set; }

		// The agent that will be occupying the tile
		public Agent Agent { get; set; }

		// Where the agent will go next (important in order to avoid having agents trade tiles)
		public Tile NextDestination { get; set; }
	}
}