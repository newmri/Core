using UnityCoreLibrary;
using UnityEngine;

public class LoginScene : BaseScene
{
    private void Awake()
    {
        Init(CoreDefine.Scene.Login);
    }

    protected override void Init(CoreDefine.Scene scene)
    {
        base.Init(scene);
        Managers.UI.ShowSceneUI<UILoginScene>();
        Managers.LoginData.Load();
    }

    public override void Clear()
    {
    }
}
