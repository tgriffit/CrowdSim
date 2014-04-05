using System;
using UnityEngine;

namespace Simulation
{
	public static class MeshUtilities
	{
		// Uses separating axis theorem to determine if a triangle overlaps a box.
		// SAT reference: http://dl.acm.org/citation.cfm?id=1198747
		// SAT code		: http://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/tribox3.txt
		public static bool TestTriangleBoxOverlap(Vector3 boxCenter, Vector3 boxHalfSize, Vector3[] vertices)
		{
			// Move the triangle into the box's local coordinates
			vertices[0] -= boxCenter;
			vertices[1] -= boxCenter;
			vertices[2] -= boxCenter;
			
			// Find the triangle's edges
			Vector3 edge0 = vertices[1] - vertices[0];
			Vector3 edge1 = vertices[2] - vertices[1];
			Vector3 edge2 = vertices[0] - vertices[2];

																// x=0, y=1, z=2
			if (AxisTest(edge0, vertices[0], vertices[2], boxHalfSize, 1, 2)) return false;
			if (AxisTest(edge0, vertices[0], vertices[2], boxHalfSize, 2, 0)) return false;
			if (AxisTest(edge0, vertices[1], vertices[2], boxHalfSize, 0, 1)) return false;

			if (AxisTest(edge1, vertices[0], vertices[2], boxHalfSize, 1, 2)) return false;
			if (AxisTest(edge1, vertices[0], vertices[2], boxHalfSize, 2, 0)) return false;
			if (AxisTest(edge1, vertices[0], vertices[1], boxHalfSize, 0, 1)) return false;

			if (AxisTest(edge2, vertices[0], vertices[1], boxHalfSize, 1, 2)) return false;
			if (AxisTest(edge2, vertices[0], vertices[1], boxHalfSize, 2, 0)) return false;
			if (AxisTest(edge2, vertices[1], vertices[2], boxHalfSize, 0, 1)) return false;

			// Test overlap in x direction
			float min = Math.Min(vertices[0].x, Math.Min(vertices[1].x, vertices[2].x));
			float max = Math.Max(vertices[0].x, Math.Max(vertices[1].x, vertices[2].x));
			if (min > boxHalfSize.x || max < -boxHalfSize.x) return false;

			// Test overlap in y direction
			min = Math.Min(vertices[0].y, Math.Min(vertices[1].y, vertices[2].y));
			max = Math.Max(vertices[0].y, Math.Max(vertices[1].y, vertices[2].y));
			if (min > boxHalfSize.y || max < -boxHalfSize.y) return false;

			// Test overlap in z direction
			min = Math.Min(vertices[0].z, Math.Min(vertices[1].z, vertices[2].z));
			max = Math.Max(vertices[0].z, Math.Max(vertices[1].z, vertices[2].z));
			if (min > boxHalfSize.z || max < -boxHalfSize.z) return false;

			Vector3 normal = Vector3.Cross(edge0, edge1);
			return PlaneBoxOverlap(normal, vertices[0], boxHalfSize);
		}

		private static bool AxisTest(Vector3 edge, Vector3 va, Vector3 vb, Vector3 boxSize, int firstAxis, int secondAxis)
		{
			float a = edge[secondAxis];
			float b = edge[firstAxis];

			float p0 = a * va[firstAxis] - b * va[secondAxis];
			float p1 = a * vb[firstAxis] - b * vb[secondAxis];

			float max = Math.Max(p0, p1);
			float min = Math.Min(p0, p1);

			float radius = Math.Abs(a) * boxSize[firstAxis] + Math.Abs(b) * boxSize[secondAxis];

			return (min > radius || max < -radius);
		}

		private static float GetAxisValue(Vector3 v, string axisName)
		{
			var axis = typeof(Vector3).GetProperty(axisName);

			// C# casting can be a strange beast
			return Convert.ToSingle(axis.GetValue(v, null));
		}

		private static bool PlaneBoxOverlap(Vector3 normal, Vector3 point, Vector3 size)
		{
			float v;
			Vector3 min = new Vector3();
			Vector3 max = new Vector3();

			for (int i = 0; i < 3; ++i)
			{
				v = point[i];

				if (normal[i] > 0)
				{
					min[i] = -size[i] - v;
					max[i] =  size[i] - v;
				}
				else
				{
					min[i] =  size[i] - v;
					max[i] = -size[i] - v;
				}
			}

			if (Vector3.Dot(normal, min) > 0) return false;
			if (Vector3.Dot(normal, max) >= 0) return true;

			return false;
		}
	}
}

