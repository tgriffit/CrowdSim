using System;
using UnityEngine;

namespace Simulation
{
	public class InteractionHandler
	{
		private const int buffer = 10;
		private const int boxWidth = 120;

		private static int currentPos;

		private static Vector3 cameraAngles = new Vector3();

		public static void InitializeCamera()
		{
			cameraAngles = Camera.main.transform.rotation.eulerAngles;
		}

		public static void Update()
		{
			CheckPopulationKeys();
			MoveCamera();
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
		private static void MoveCamera()
		{
			float movementSpeed = 0.25f;
			float rotateSpeed = 2.0f;

			Vector3 deltaPos = new Vector3();

			// Move the camera based on the buttons defined as controlling movement in Unity
			deltaPos.x += Input.GetAxisRaw("Horizontal") * movementSpeed;
			deltaPos.z += Input.GetAxisRaw("Vertical") * movementSpeed;

			Camera.main.transform.Translate(deltaPos);

			// If the right mouse button is held down we should rotate the camera
			if (Input.GetMouseButton(1))
			{
				cameraAngles.y +=  Input.GetAxisRaw("Mouse X") * rotateSpeed;
				cameraAngles.x += -Input.GetAxisRaw("Mouse Y") * rotateSpeed;
			}

			// This gimbal locks the camera, but that's the desired behaviour
			Camera.main.transform.rotation = Quaternion.Euler(cameraAngles);
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
