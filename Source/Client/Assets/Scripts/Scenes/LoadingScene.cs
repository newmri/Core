using System.Collections;
using UnityCoreLibrary;
using UnityEngine;

public class LoadingScene : BaseScene
{
    private void Awake()
    {
        Init(CoreDefine.Scene.Loading);
    }

    protected override void Init(CoreDefine.Scene scene)
    {
        base.Init(scene);
        Managers.UI.ShowSceneUI<UILoadingScene>();
    }

    public override void Clear()
    {
    }
}
