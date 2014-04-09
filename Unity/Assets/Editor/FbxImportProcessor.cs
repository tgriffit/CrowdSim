/*
 * This code for linking textures with imported models automatically was taken from:
 * http://bilke.info/blog/2013/2/5/import-fbx-meshes-with-embedded-textures-into-unity
 */

using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
using System.IO;

public class FbxImportProcessor : AssetPostprocessor
{

	void OnPostprocessTexture(Texture2D texture)
	{
		Debug.Log("Testing");
		if (assetPath.Contains(".fbm"))
		{
			Debug.Log("Passed");
			string modelName = Path.GetFileName(assetPath.Substring(0, assetPath.LastIndexOf(".fbm")));
			string basePath = Path.GetDirectoryName(assetPath.Substring(0, assetPath.LastIndexOf(".fbm")));
			string materialPath = basePath + "/Materials/" + modelName + ".mat";
			Material material = (Material)(AssetDatabase.LoadAssetAtPath(materialPath, typeof(Material))); ;
			if (material)
			{
				Texture2D tex = (Texture2D)(AssetDatabase.LoadAssetAtPath(assetPath, typeof(Texture2D)));
				if (tex)
					material.mainTexture = tex;
				else
				{
					AssetDatabase.Refresh();
					tex = (Texture2D)(AssetDatabase.LoadAssetAtPath(assetPath, typeof(Texture2D)));
					if (tex)
						material.mainTexture = tex;
				}
			}
			else
				Debug.Log("Error: material not found for " + assetPath);
		}
	}
}