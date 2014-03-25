using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public static class Utilities
{
	// Returns a random entry from the collection
	public static T GetRandomEntry<T>(IEnumerable<T> collection)
	{
		int index = Random.Range(0, collection.Count());
		return collection.ElementAt(index);
	}
}
