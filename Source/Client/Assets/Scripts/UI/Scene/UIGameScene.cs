using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using Define;
using GamePacket;

public class UIGameScene : UIScene
{
    enum Sliders
    {
        HPSlider,
        MPSlider
    }

    enum TextMeshProUGUIs
    {
        CharacterLevelText,
        CharacterNameText,
        GemText,
        GoldText
    }

    enum Images
    {
        JobIconImage
    }

    public override void Init()
    {
        Managers.UI.SetCanvas(gameObject);

        Bind<Slider>(typeof(Sliders));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
        Bind<Image>(typeof(Images));

        UpdateHPBar();
        UpdateHPBar();

        UpdateCharacterInfo();
        UpdateMoney();
    }

    public void UpdateHPBar()
    {
        var info = Managers.Character.GetMyCharacterInfo();
        GetSlider((int)Sliders.HPSlider).value = info.Hp / info.Ability.Value[(int)AbilityByStatType.HP];
    }

    public void UpdateMPBar()
    {
        var info = Managers.Character.GetMyCharacterInfo();
        GetSlider((int)Sliders.MPSlider).value = info.Mp / info.Ability.Value[(int)AbilityByStatType.MP];
    }

    public void UpdateCharacterInfo()
    {
        MyCharacterInfoT info = Managers.Character.GetMyCharacterInfo();

        this.GetTextMesh((int)TextMeshProUGUIs.CharacterNameText).text = info.Name;
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterLevelText).text = "Lv." + info.Level.ToString();

        Sprite jobIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{info.Job.ToString()}Icon");
        GetImage((int)Images.JobIconImage).sprite = jobIcon;
    }

    public void UpdateMoney()
    {
        this.GetTextMesh((int)TextMeshProUGUIs.GemText).text = Managers.Account.Money.Value[(int)MoneyType.GEM].ToString();
        this.GetTextMesh((int)TextMeshProUGUIs.GoldText).text = Managers.Account.Money.Value[(int)MoneyType.GOLD].ToString();
    }
}
