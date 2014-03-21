using System;
using UnityEngine;

public class InteractionHandler
{
	public static void Update()
	{

	}

	public static void OnGUI()
	{
		HandlePopulation();
	}

	private static void HandlePopulation()
	{
		GUI.Box(new Rect(10, 10, 110, 90), String.Format("Population: {0}", Simulation.Instance.Population));

		if (GUI.Button(new Rect(20, 40, 40, 20), "(+)"))
		{
			Simulation.Instance.ChangePopulation(1);
		}

		if (GUI.Button(new Rect(70, 40, 40, 20), "(+10)"))
		{
			Simulation.Instance.ChangePopulation(10);
		}

		if (GUI.Button(new Rect(20, 70, 40, 20), "(-)"))
		{
			Simulation.Instance.ChangePopulation(-1);
		}
		
		if (GUI.Button(new Rect(70, 70, 40, 20), "(-10)"))
		{
			Simulation.Instance.ChangePopulation(-10);
		}
	}
}

