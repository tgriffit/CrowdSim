/**
 * This code was originally written by Rod Green and can be found at http://wiki.unity3d.com/index.php?title=AStarHelper.
 */

using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace Simulation
{
	public static class AStarHelper
	{
		
		
		// Validator for path nodes
		// Needed to cope with nodes that might be GameObjects and therefore
		// not 'acutally' null when compared in generic methods
		public static bool Invalid<T>(T inNode) where T : IPathNode<T>
		{
			if(inNode == null || inNode.Invalid)
				return true;
			return false;
		}
		
		// Distance between Nodes
		static float Distance<T>(T start, T goal, int time, float speed) where T : IPathNode<T>
		{
			if(Invalid(start) || Invalid(goal))
				return float.MaxValue;
			return start.GetDistance(goal, time, speed);
		}
		
		// Base cost Estimate - this would need to be evoled for your project based on true cost
		// to move between nodes
		static float HeuristicCostEstimate<T>(T start, T goal, float speed) where T: IPathNode<T>
		{
			return start.GetDistanceHeuristic(goal, speed);
		}
		
		// Find the current lowest score path
		static T LowestScore<T>(List<T> openset, Dictionary<T, float> scores) where T : IPathNode<T>
		{
			int index = 0;
			float lowScore = float.MaxValue;
			
			for(int i = 0; i < openset.Count; i++)
			{
				if(scores[openset[i]] > lowScore)
					continue;
				index = i;
				lowScore = scores[openset[i]];
			}
			
			return openset[index];
		}
		
		
		// Calculate the A* path
		public static List<IPathAction<T>> Calculate<T>(T start, T goal, float speed) where T : IPathNode<T>
		{
			List<T> closedset = new List<T>();  // The set of nodes already evaluated.
			List<T> openset = new List<T>();    // The set of tentative nodes to be evaluated.
			openset.Add(start);
			Dictionary<T, T> cameFrom = new Dictionary<T, T>();    // The map of navigated nodes.
			
			Dictionary<T, float> gScore = new Dictionary<T, float>();
			gScore[start] = 0.0f; // Cost from start along best known path.
			
			Dictionary<T, float> hScore = new Dictionary<T, float>();
			hScore[start] = HeuristicCostEstimate(start, goal, speed); 
			
			Dictionary<T, float> fScore = new Dictionary<T, float>();
			fScore[start] = hScore[start]; // Estimated total cost from start to goal through y.
			
			while(openset.Count != 0)
			{
				T x = LowestScore(openset, fScore);
				if(x.Equals(goal))
				{
					int time = 0;

					List<IPathAction<T>> result = new List<IPathAction<T>>();
					ReconstructPath(cameFrom, x, default(T), speed, ref result, ref time);
					return result;
				}
				openset.Remove(x);
				closedset.Add(x);

				foreach(T y in x.Connections)
				{
					if(AStarHelper.Invalid(y) || closedset.Contains(y))
						continue;
					float tentativeGScore = gScore[x] + Distance(x, y, (int)(gScore[x] / speed), speed);
					
					bool tentativeIsBetter = false;
					if(!openset.Contains(y))
					{
						openset.Add(y);
						tentativeIsBetter = true;
					}
					else if (tentativeGScore < gScore[y])
						tentativeIsBetter = true;
					
					if(tentativeIsBetter)
					{
						cameFrom[y] = x;
						gScore[y] = tentativeGScore;
						hScore[y] = HeuristicCostEstimate(y, goal, speed);
						fScore[y] = gScore[y] + hScore[y];
					}
				}
			}

			return null;
		}

		// Once the goal has been found we now reconstruct the steps taken to get to the path
		static void ReconstructPath<T>(Dictionary<T, T> cameFrom, T currentNode, T previousNode, float speed, ref List<IPathAction<T>> result, ref int time) where T : IPathNode<T>
		{
			if (cameFrom.ContainsKey(currentNode))
			{
				ReconstructPath(cameFrom, cameFrom[currentNode], currentNode, speed, ref result, ref time);
			}

			if (previousNode != null && result != null) 
			{
				var path = currentNode.GetPathTo(previousNode, speed, ref time);

				if (path == null)
				{
					result = null;
					return;
				}

				result.AddRange(path);
			}
		}
	}
}