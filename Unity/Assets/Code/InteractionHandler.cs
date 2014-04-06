using System;
using UnityEngine;

namespace Simulation
{
	public class InteractionHandler
	{
		private const int buffer = 10;
		private const int boxWidth = 120;

		private static int currentPos;

		public static void Update()
		{
			CheckPopulationKeys();
			CheckCameraKeys();
		}

		public static void OnGUI()
		{
			currentPos = 0;

			currentPos += buffer;
			CreatePopulationGUI();
			
			currentPos += buffer;
			CreatePauseGUI();
		}

		// Checks the keyboard commands that modify population
		private static void CheckPopulationKeys()
		{
			if (Input.GetKeyDown(KeyCode.KeypadPlus) || Input.GetKeyDown(KeyCode.Equals))
			{
				Simulation.Instance.MaxPopulation += 1;
			}

			if (Input.GetKeyDown(KeyCode.KeypadMultiply) || Input.GetKeyDown(KeyCode.Alpha8))
			{
				Simulation.Instance.MaxPopulation += 10;
			}

			if (Input.GetKeyDown(KeyCode.KeypadMinus) || Input.GetKeyDown(KeyCode.Minus))
			{
				Simulation.Instance.MaxPopulation -= 1;
			}

			if (Input.GetKeyDown(KeyCode.KeypadDivide) || Input.GetKeyDown(KeyCode.Slash))
			{
				Simulation.Instance.MaxPopulation -= 10;
			}
		}

		// Checks to see if the user is trying to move the camera
		private static void CheckCameraKeys()
		{
			float movementAmount = 0.25f;
			Vector3 delta = new Vector3();

			if (Input.GetKeyDown(KeyCode.W))
			{
				delta.z += movementAmount;
			}
			
			if (Input.GetKeyDown(KeyCode.S))
			{
				delta.z -= movementAmount;
			}

			if (Input.GetKeyDown(KeyCode.A))
			{
				delta.x -= movementAmount;
			}

			if (Input.GetKeyDown(KeyCode.D))
			{
				delta.x += movementAmount;
			}

			if (Input.GetKeyDown(KeyCode.E))
			{
				delta.y += movementAmount;
			}

			if (Input.GetKeyDown(KeyCode.Q))
			{
				delta.y -= movementAmount;
			}

			Camera.main.transform.Translate(delta);
		}

		private static void CreatePopulationGUI()
		{
			int elemWidth = (boxWidth - 3*buffer) / 2;
			int elemHeight = 2 * buffer;
			int leftCol = 2 * buffer;					// left buffer + one from frame to element
			int rightCol = 3 * buffer + elemWidth;		// left buffer + one from frame + one between them + width of left elem
			int totalHeight = 2 * elemHeight + 5 * buffer;

			GUI.Box(new Rect(buffer, currentPos, boxWidth, totalHeight), 
				String.Format("Population: {0}/{1}", Simulation.Instance.Population, Simulation.Instance.MaxPopulation));

			currentPos += 3 * buffer;					// Add padding at the top + text

			if (GUI.Button(new Rect(leftCol, currentPos, elemWidth, elemHeight), "(+)"))
			{
				Simulation.Instance.MaxPopulation += 1;
			}

			if (GUI.Button(new Rect(rightCol, currentPos, elemWidth, elemHeight), "(+10)"))
			{
				Simulation.Instance.MaxPopulation += 10;
			}

			currentPos += elemHeight + buffer;

			if (GUI.Button(new Rect(leftCol, currentPos, elemWidth, elemHeight), "(-)"))
			{
				Simulation.Instance.MaxPopulation -= 1;
			}
			
			if (GUI.Button(new Rect(rightCol, currentPos, elemWidth, elemHeight), "(-10)"))
			{
				Simulation.Instance.MaxPopulation -= 10;
			}
			
			currentPos += elemHeight + buffer;
		}

		private static void CreatePauseGUI()
		{
			GUI.Box(new Rect(buffer, currentPos, boxWidth, 4 * buffer), String.Empty);

			currentPos += buffer;

			if (GUI.Button(new Rect(2 * buffer, currentPos, boxWidth - 2*buffer, 2*buffer), Simulation.Instance.Playing ? "Pause" : "Play"))
			{
				Simulation.Instance.Playing = !Simulation.Instance.Playing;
			}
		}
	}
}
