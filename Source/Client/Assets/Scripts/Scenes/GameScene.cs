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
        Managers.AbilityData.Load();
        Managers.EffectData.Load();
        Managers.GradeData.Load();

        Managers.Account.UIGameScene = Managers.UI.ShowSceneUI<UIGameScene>();
        Managers.Object.AddMyPlayer();
    }

    public override void Clear()
    {
        Managers.Clear();
        CoreManagers.Clear();
    }
}
