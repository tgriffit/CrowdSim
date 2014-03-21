using UnityEngine;
using System.Collections;
using System.Collections.Generic;

/*
 *	This script controls which agents are spawned.
 *	There should be an instance of this script in any
 *	scene in which you want to spawn agents.
 */
public class AgentSpawner : MonoBehaviour
{
	public GameObject[] models;
	public static AgentSpawner Instance = null;

	// Use this for initialization
	void Start()
	{
		if (models == null || models.Length == 0) 
		{
			Debug.LogError("No spawnable models found by AgentSpawner!");
		}

		if (Instance == null) 
		{
			Instance = this;
		}
		else 
		{
			Debug.LogError("Multiple AgentSpawners found in level!");
		}
	}
	
	// Update is called once per frame
	void Update()
	{
	}

	public static Agent GetAgent()
	{
		// Returns an agent with a random appearance from the list
		return new Agent(Instance.models[Random.Range(0, Instance.models.Length)]);
	}
}
