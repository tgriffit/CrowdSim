using UnityEngine;
using System.Collections;

/*
 *	This script controls which agents are spawned.
 *	There should be an instance of this script in any
 *	scene in which you want to spawn agents.
 */
public class AgentSpawner : MonoBehaviour
{
	public static Transform[] models;

	// Use this for initialization
	void Start()
	{
		if (models.Length == 0) 
		{
			Debug.LogError("No spawnable models found by AgentSpawner!");
		}
	}
	
	// Update is called once per frame
	void Update()
	{
	}

	public static Agent GetAgent()
	{
		// Returns an agent with a random appearance from the list
		return new Agent(models[Random.Range(0, models.Length - 1)]);
	}
}
