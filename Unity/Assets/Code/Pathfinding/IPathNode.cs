/**
 * This code was originally written by Rod Green and can be found at http://wiki.unity3d.com/index.php?title=AStarHelper.
 */

using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace Simulation
{
	public interface IPathNode<T>
	{
		List<T> Connections { get; }
		Vector3 Position { get; }
		bool Invalid { get; }

		float GetDistanceHeuristic(T other, float speed);
		float GetDistance(T other, int time, float speed);

		// Find the list of actions required to pass from one node to another
		List<IPathAction<T>> GetPathTo(T other, float speed, ref int time);
	}
}