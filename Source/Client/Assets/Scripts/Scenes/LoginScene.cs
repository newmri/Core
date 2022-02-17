using UnityCoreLibrary;
using UnityEngine;

public class LoginScene : BaseScene
{
    private void Awake()
    {
        Init(Define.Scene.Login);
    }

    protected override void Init(Define.Scene scene)
    {
        base.Init(scene);
        CoreManagers.UI.ShowSceneUI<UILoginScene>("UILoginScene");
    }

    public override void Clear()
    {
    }
}
