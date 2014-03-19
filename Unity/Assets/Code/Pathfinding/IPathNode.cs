/**
 * This code was originally written by Rod Green and can be found at http://wiki.unity3d.com/index.php?title=AStarHelper.
 */

using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public interface IPathNode<T>
{
	List<T> Connections { get; }
	Vector3 Position { get; }
	bool Invalid { get; }

	float GetDistanceHeuristic(T other);
}