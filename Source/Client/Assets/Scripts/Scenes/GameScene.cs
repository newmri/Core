using UnityCoreLibrary;
using UnityEngine;

public class GameScene : BaseScene
{
    private void Awake()
    {
        Init(CoreDefine.Scene.Game);
    }

    protected override void Init(CoreDefine.Scene scene)
    {
        base.Init(scene);
        Managers.Map.LoadMap(1);
    }

    public override void Clear()
    {
    }
}