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
	const int TILESIZE = 1;
	Vector3 corner;
	int[][] tiles;

	// Use this for initialization
	void Start ()
	{
		Vector3 center = gameObject.transform.position;
		Vector3 size = gameObject.transform.localScale;

		corner = new Vector3 (center.x - size.x / 2, center.y + size.y / 2 + 1, center.z - size.z / 2);

		int numtiles = (int)size.x / TILESIZE;
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
				Vector3 upperLeft = corner + new Vector3(x * TILESIZE, 0, z * TILESIZE);
				Vector3 upperRight = corner + new Vector3((x+1) * TILESIZE, 0, z * TILESIZE);
				Vector3 lowerLeft = corner + new Vector3(x * TILESIZE, 0, (z+1) * TILESIZE);
				Vector3 lowerRight = corner + new Vector3((x+1) * TILESIZE, 0, (z+1) * TILESIZE);
				
				Debug.DrawLine(upperLeft, upperRight, Color.red);
				Debug.DrawLine(upperRight, lowerRight, Color.red);
				Debug.DrawLine(lowerRight, lowerLeft, Color.red);
				Debug.DrawLine(lowerLeft, upperLeft, Color.red);
			}
		}
	}
}
