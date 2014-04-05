using System;

namespace Simulation
{
	public class TileClaim
	{
		// How many frames until the agent arrives in the tile
		public int StartTime { get; set; }

		// How many frames until the agent is no longer in the tile
		public int EndTime { get; set; }
	}
}