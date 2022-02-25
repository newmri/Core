using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using System;

public class UICharacterEmptySlot : UIPopup
{
    enum Buttons
    {
        UICharacterEmptySlot
    }

    public override void Init()
    {
        base.Init();
        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.UICharacterEmptySlot).gameObject.BindEvent(OnClickCreateCharacterButton);
    }

    public void OnClickCreateCharacterButton(PointerEventData evt)
    {
        Debug.Log("Create");
    }
}
