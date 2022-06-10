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
    }

    public override void Clear()
    {
    }
}
