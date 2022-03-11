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
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
        Bind<Image>(typeof(Images));

        InitCharacterInfo();
        InitMoney();
    }

    void InitCharacterInfo()
    {
        CharacterInfoT info = Managers.Character.GetMyCharacterInfo();

        this.GetTextMesh((int)TextMeshProUGUIs.CharacterNameText).text = info.Name;
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterLevelText).text = "Lv." + info.Level.ToString();

        Sprite jobIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{info.Job.ToString()}Icon");
        GetImage((int)Images.JobIconImage).sprite = jobIcon;
    }

    void InitMoney()
    {
        this.GetTextMesh((int)TextMeshProUGUIs.GemText).text = Managers.Account.Money[(int)Money.GEM].ToString();
        this.GetTextMesh((int)TextMeshProUGUIs.GoldText).text = Managers.Account.Money[(int)Money.GOLD].ToString();
    }
}
