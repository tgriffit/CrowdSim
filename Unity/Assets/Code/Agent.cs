using System;
using UnityEngine;

public class Agent
{
	private Transform model;

	public Agent (Transform m)
	{
		this.model = m;
	}

	public void spawn(Vector3 position, Vector3 direction)
	{
		UnityEngine.Object.Instantiate(model, position, Quaternion.LookRotation(direction));
	}
}

