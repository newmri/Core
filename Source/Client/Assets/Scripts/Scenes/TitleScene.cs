using UnityCoreLibrary;
using UnityEngine;

public class TitleScene : BaseScene
{
    private void Awake()
    {
        Init(Define.Scene.Title);
    }

    public GameObject Root
    {
        get
        {
            GameObject root = GameObject.Find("@UI_Root");
            if (root == null)
                root = new GameObject { name = "@UI_Root" };

            return root;
        }
    }

    protected override void Init(Define.Scene scene)
    {
        base.Init(scene);
        CoreManagers.UI.ShowSceneUI<UITitleScene>("UITitleScene");
    }

    public override void Clear()
    {
    }
}
