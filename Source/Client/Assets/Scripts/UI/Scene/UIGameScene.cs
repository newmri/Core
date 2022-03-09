using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using GamePacket;

public class UIGameScene : UIScene
{
    enum TextMeshProUGUIs
    {
        CharacterLevelText,
        CharacterNameText,
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
    }

    void InitCharacterInfo()
    {
        CHARACTER_INFOT info = Managers.Character.GetMyCharacterInfo();

        this.GetTextMesh((int)TextMeshProUGUIs.CharacterNameText).text = info.Name;
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterLevelText).text = "Lv." + info.Level.ToString();

        Sprite jobIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{info.Job.ToString()}Icon");
        GetImage((int)Images.JobIconImage).sprite = jobIcon;
    }
}
