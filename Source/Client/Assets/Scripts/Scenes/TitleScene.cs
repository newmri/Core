using UnityCoreLibrary;
using UnityEngine;

public class TitleScene : BaseScene
{
    private void Awake()
    {
        Init(Define.Scene.Title);
    }

    protected override void Init(Define.Scene scene)
    {
        base.Init(scene);
        Managers.UI.ShowSceneUI<UITitleScene>();
    }

    public override void Clear()
    {
    }
}
