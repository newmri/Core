using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using System;

public class UIWorldListPopupInfo : UIPopup
{
    [SerializeField]
    Sprite[] _swapImage = new Sprite[2];

    public bool IsSelected { get; set; }
    public WorldListInfo Info { get; set; }

    enum GameObjects
    {
        WorldName,
        BusyScore
    }

    enum Buttons
    {
        WorldInfoButton
    }

    public override void Init()
    {
        base.Init();
        Bind<GameObject>(typeof(GameObjects));
        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.WorldInfoButton).gameObject.BindEvent(OnClickButton);
        Get<GameObject>((int)GameObjects.WorldName).GetComponent<TextMeshProUGUI>().text = Info.Name;

        string busyScore;
        if (Info.BusyScore >= 70)
            busyScore = "<color=red>È¥Àâ</color>";
        else
            busyScore = "<color=green>¿øÇÒ</color>";

        Get<GameObject>((int)GameObjects.BusyScore).GetComponent<TextMeshProUGUI>().text = busyScore;
    }


    public void OnClickCloseButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI(true);
    }

    void OnClickButton(PointerEventData evt)
    {
        IsSelected = !IsSelected;
        GetButton((int)Buttons.WorldInfoButton).GetComponent<Image>().sprite = _swapImage[Convert.ToInt32(IsSelected)];
    }
}
