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
		var meshFilter = gameObject.GetComponent<MeshFilter>();
		if (meshFilter == null)
		{
			Debug.LogError("There's no Mesh Filter attached to this component! " +
			               "This script should be added to the portion of the model that controls the mesh.");
		}
		var mesh = meshFilter.mesh;

		Vector3 center = gameObject.transform.position;
		Vector3 size = gameObject.renderer.bounds.size;

		Vector3 corner = new Vector3(center.x - size.x / 2, findGroundLevel(mesh.normals), center.z - size.z / 2);

		Grid.Instance.setup(corner, (int)(size.x / Tile.TILESIZE), (int)(size.z / Tile.TILESIZE), mesh.vertices, mesh.triangles);

		Simulation.Start();
	}
	
	// Update is called once per frame
	void Update()
	{
		Grid.Instance.DebugDraw();
		Simulation.Update();
	}

	private float findGroundLevel(Vector3[] normals)
	{
		// Dark secret - we just assume that the ground is at height 0. 
		// If this becomes a problem then we may be able to come up with a solution, but it's problematic.

		return 0.0f;
	}
}
