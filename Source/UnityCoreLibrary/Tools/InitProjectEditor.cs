using UnityEngine;
using System.IO;
using UnityEngine.EventSystems;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace UnityCoreLibrary.Init
{
	public class InitProjectEditor
	{
#if UNITY_EDITOR
	// % (Ctrl), # (Shift), & (Alt)

	[MenuItem("Tools/InitProject %#g")]
	private static void InitProject()
	{
		CreateFolders();
	}

	private static void CreateFolders()
	{
		CreateFolder("Assets/Plugins");
		CreateFolder("Assets/Resources");
		CreateFolder("Assets/Resources/Animations");
		CreateFolder("Assets/Resources/Effects");
		CreateFolder("Assets/Resources/Maps");
		CreateFolder("Assets/Resources/Prefabs");
		CreateFolder("Assets/Resources/Sounds");
		CreateFolder("Assets/Resources/UI");
		CreateFolder("Assets/Resources/Sprites");

		CreateEventSystem();

		CreateFolder("Assets/Scripts");
		CreateFolder("Assets/Scripts/Controllers");
		CreateFolder("Assets/Scripts/Define");
		CreateFolder("Assets/Scripts/Managers");
		CreateFolder("Assets/Scripts/Managers/Core");
		CreateFolder("Assets/Scripts/Managers/Contents");
		CreateFolder("Assets/Scripts/Map");
		CreateFolder("Assets/Scripts/Objects");
		CreateFolder("Assets/Scripts/Scenes");
		CreateFolder("Assets/Scripts/UI");
		CreateFolder("Assets/Scripts/Data");

		CreateFolder("Assets/ThirdParty");
	}

	private static void CreateFolder(string path)
	{
	    if (!Directory.Exists(path))
        {
            Directory.CreateDirectory(path);
        }
	}

	[System.Obsolete]
	private static void CreateEventSystem()
	{
	    GameObject go = new GameObject("EventSystem");
        go.GetOrAddComponent<EventSystem>();
        go.GetOrAddComponent<StandaloneInputModule>();
		CreateNewPrefab(go, "Assets/Resources/Prefabs/UI/", "EventSystem");
		GameObject.DestroyImmediate(go);
	}

	[System.Obsolete]
	private static void CreateNewPrefab(GameObject obj, string path, string name)
    {
        CreateFolder(path);

        PrefabUtility.CreatePrefab(path + name + ".prefab", obj);
		AssetDatabase.Refresh();
    }
#endif
	}
}
