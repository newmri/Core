using UnityEngine;
using UnityCoreLibrary;
using UnityEngine.SceneManagement;

public class SceneManagerEx
{
    public BaseScene CurrentScene { get { return GameObject.FindObjectOfType<BaseScene>(); } }

    public void LoadScene(CoreDefine.Scene type)
    {
        CoreManagers.Clear();

        SceneManager.LoadScene(GetSceneName(type));
    }

    string GetSceneName(CoreDefine.Scene type)
    {
        string name = System.Enum.GetName(typeof(CoreDefine.Scene), type);
        return name;
    }

    public void Clear()
    {
        CurrentScene.Clear();
    }
}