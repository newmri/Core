using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;
using System.IO;
using System;
using UnityCoreLibrary;

#if UNITY_EDITOR
using UnityEditor;
#endif

public class MapEditor
{
#if UNITY_EDITOR
	// % (Ctrl), # (Shift), & (Alt)

	[MenuItem("Tools/GenerateMap %#g")]
	private static void GenerateMap()
	{
		GenerateByPath("Assets/Resources/Map/Data");
	}

	private static void GenerateByPath(string pathPrefix)
	{
		GameObject[] gameObjects = Resources.LoadAll<GameObject>("Prefabs/Map");

		foreach (GameObject go in gameObjects)
		{
			Tilemap tileMapBase = Util.FindChild<Tilemap>(go, "Tilemap_Base", true);
			Tilemap tileMapPath = Util.FindChild<Tilemap>(go, "Tilemap_Path", true);

			using (var writer = File.CreateText($"{pathPrefix }/{go.name}.txt"))
			{
				WriteMinMax(writer, tileMapBase);

				for (int y = tileMapBase.cellBounds.yMax + _maxCorrections.y; y >= tileMapBase.cellBounds.yMin + _minCorrections.y; --y)
				{
					for (int x = tileMapBase.cellBounds.xMin + _minCorrections.x; x <= tileMapBase.cellBounds.xMax + _maxCorrections.x; ++x)
					{
						WritePath(writer, tileMapPath.GetTile(new Vector3Int(x, y, 0)));
					}

					writer.WriteLine();
				}
			}
		}
	}

	private static void WriteMinMax(StreamWriter writer, Tilemap tileMapBase)
	{
		writer.WriteLine(tileMapBase.cellBounds.xMin + _minCorrections.x);
		writer.WriteLine(tileMapBase.cellBounds.yMin + _minCorrections.y);
		writer.WriteLine(tileMapBase.cellBounds.xMax + _maxCorrections.x);
		writer.WriteLine(tileMapBase.cellBounds.yMax + _maxCorrections.y);
	}

	private static void WritePath(StreamWriter writer, TileBase tileBase)
	{
		if (null == tileBase)
		{
			writer.Write((int)Path.None);
		}
		else
		{
			var type = typeof(Path);
			string name = tileBase.name;

			if (type.ToString() != name)
				name = name.Replace("Path", "");

			Path path;
			if (Enum.TryParse<Path>(name, out path))
			{
				writer.Write((int)path);
			}
		}
	}

	private static Vector2Int _minCorrections = new Vector2Int(0, 0);
	private static Vector2Int _maxCorrections = new Vector2Int(-1, -1);

#endif
}