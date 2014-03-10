using UnityEngine;
using System.Collections;

/*
This script divides a rectangular piece of ground into 1 m sections.
Bad Assumptions:
	The surface is a perfect rectangle.
	The surface has uniform y values.
	The surface is contiguous.
*/

public class PartitionTerrain : MonoBehaviour
{
	const float TILESIZE = 1.0f;
	Vector3 corner;
	int[][] tiles;

	// Use this for initialization
	void Start ()
	{
		Vector3 center = gameObject.transform.position;

		// This should instead be based on the mesh bounds. This only really works on a simple cube,
		// since it's size is equal to it's scale.
		Vector3 size = gameObject.transform.localScale;

		corner = new Vector3 (center.x - size.x / 2, center.y + size.y / 2, center.z - size.z / 2);

		int numtiles = (int)(size.x / TILESIZE);
		tiles = new int[numtiles][];
		for (int i = 0; i < tiles.Length; ++i)
		{
			tiles [i] = new int[numtiles];
		}
	}

	// Update is called once per frame
	void Update ()
	{
		// This is horribly inefficient, but it's just test code

		for (int x = 0; x < tiles.Length; ++x)
		{
			for (int z = 0; z < tiles[x].Length; ++z)
			{
				// Draw 4 lines to represent the borders of the tile
				Vector3 upperLeft = corner + new Vector3 (x * TILESIZE, 0, z * TILESIZE);
				Vector3 upperRight = corner + new Vector3 ((x + 1) * TILESIZE, 0, z * TILESIZE);
				Vector3 lowerLeft = corner + new Vector3 (x * TILESIZE, 0, (z + 1) * TILESIZE);
				Vector3 lowerRight = corner + new Vector3 ((x + 1) * TILESIZE, 0, (z + 1) * TILESIZE);

				Debug.DrawLine (upperLeft, upperRight, Color.white);
				Debug.DrawLine (upperRight, lowerRight, Color.white);
				Debug.DrawLine (lowerRight, lowerLeft, Color.white);
				Debug.DrawLine (lowerLeft, upperLeft, Color.white);

				Debug.DrawLine (upperLeft, lowerRight, getTileColor(x, z));
			}
		}
	}

	// Marks the tile debug marker based on the tile's walkablility
	Color getTileColor (int x, int z)
	{
		// Right now, this justs tests for evenness because nothing else is in place.
		if (x % 2 == 0 && z % 2 == 0)
		{
			return Color.red;
		}
		else if (x % 2 == 0)
		{
			return Color.yellow;
		}
		else if (z % 2 == 0)
		{
			return Color.cyan;
		}
		else
		{
			return Color.clear;
		}
	}
}
