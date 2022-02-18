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
        Managers.UI.ShowSceneUI<UILoginScene>();
    }

    public override void Clear()
    {
    }
}
