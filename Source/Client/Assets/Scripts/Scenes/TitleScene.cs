using UnityCoreLibrary;
using UnityEngine;

public class TitleScene : BaseScene
{
    private void Awake()
    {
        Init(CoreDefine.Scene.Title);
    }

    protected override void Init(CoreDefine.Scene scene)
    {
        base.Init(scene);
        Managers.UI.ShowSceneUI<UITitleScene>();
    }

    public override void Clear()
    {
        Managers.UI.Clear();
    }
}
