using System;
using UnityEngine;

public class InteractionHandler
{
	private const int buffer = 10;
	private const int boxWidth = 120;

	private static int currentPos;

	public static void Update()
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

	public static void OnGUI()
	{
		currentPos = 0;

		currentPos += buffer;
		HandlePopulation();
		
		currentPos += buffer;
		HandlePause();
	}

	private static void HandlePopulation()
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

	private static void HandlePause()
	{
		GUI.Box(new Rect(buffer, currentPos, boxWidth, 4 * buffer), String.Empty);

		currentPos += buffer;

		if (GUI.Button(new Rect(2 * buffer, currentPos, boxWidth - 2*buffer, 2*buffer), Simulation.Instance.Playing ? "Pause" : "Play"))
		{
			Simulation.Instance.Playing = !Simulation.Instance.Playing;
		}
	}
}

