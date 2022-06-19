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

        Managers.Map.LoadMap(0);
        Managers.CharacterData.Load();
        Managers.Item.Load();
        Managers.EffectData.Load();
        Managers.Grade.Load();

        Managers.Account.UIGameScene = Managers.UI.ShowSceneUI<UIGameScene>();
        Managers.Object.AddMyPlayer();
    }

    public override void Clear()
    {
        Managers.Clear();
        CoreManagers.Clear();
    }
}
