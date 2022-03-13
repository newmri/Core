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
using Info;

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
    }

    public void UpdateHPBar(int HP, int MaxHP)
    {
        GetSlider((int)Sliders.HPSlider).value = HP / MaxHP;
    }

    public void UpdateMPBar(int MP, int MaxMP)
    {
        GetSlider((int)Sliders.MPSlider).value = MP / MaxMP;
    }

    public void UpdateCharacterInfo(string name, byte level, Define.Job job)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterNameText).text = name;
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterLevelText).text = "Lv." + level.ToString();

        Sprite jobIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{job.ToString()}Icon");
        GetImage((int)Images.JobIconImage).sprite = jobIcon;
    }

    public void UpdateMoney(MoneyT money)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.GemText).text = money.Value[(int)MoneyType.GEM].ToString();
        this.GetTextMesh((int)TextMeshProUGUIs.GoldText).text = money.Value[(int)MoneyType.GOLD].ToString();
    }
}
