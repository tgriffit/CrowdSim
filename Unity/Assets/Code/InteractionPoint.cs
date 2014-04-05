using UnityEditor;
using UnityEngine;
using System.Collections;

namespace Simulation
{
	public class InteractionPoint : MonoBehaviour
	{
		// These give the size of the "halo" around the closest tile to the entrance
		public int xsize = 0;
		public int zsize = 0;

		// Use this for initialization
		void Start()
		{
			// Hides the point at runtime, but lets it show up nicely
			// in the editor window.
			gameObject.renderer.enabled = false;
		}
		
		// Update is called once per frame
		void Update()
		{
		
		}

		void OnDrawGizmos()
		{
			Handles.Label(transform.position, GetDisplayText());
		}

		protected virtual string GetDisplayText() 
		{
			return "Access Point"; 
		}

		// Short-hand to get the position of the object we're attached to.
		public Vector3 Position
		{
			get { return gameObject.transform.position; }
		}
	}
}