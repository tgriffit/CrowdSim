using UnityEngine;
using System.Collections;
using System.Linq;

/*
 * Initializes the pathfinding grid. This should only be run once!
 * 
 * This behaviour should be added to the piece of terrain that we'll
 * be pathfinding through.
 */

public class SetupEnvironment : MonoBehaviour
{
	
	// Use this for initialization
	void Start()
	{
		Vector3 center = gameObject.transform.position;
		Vector3 size = gameObject.renderer.bounds.size;

		// The y component of this will have to change. We need a way to figure out the the height of the flat component.
		Vector3 corner = new Vector3(center.x - size.x / 2, center.y + size.y / 2, center.z - size.z / 2);

		Grid.Instance.setup(corner, (int)(size.x / Tile.TILESIZE), (int)(size.z / Tile.TILESIZE));

		// Walk through the grid and set up the tile properties
	}
	
	// Update is called once per frame
	void Update()
	{
		Grid.Instance.DebugDraw();
	}
}
