using System.Collections;
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
        Managers.Account.UIGameScene = Managers.UI.ShowSceneUI<UIGameScene>();
        Managers.Creature.AddMyPlayer();
    }

    public override void Clear()
    {
    }
}
