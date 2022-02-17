using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class UILoginScene : UIScene
{
    enum Buttons
    {
        GuestLoginButton
    }

    public override void Init()
    {
        base.Init();

        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.GuestLoginButton).gameObject.BindEvent(OnClickStartButton);
    }

    public void OnClickStartButton(PointerEventData evt)
    {
        CoreManagers.UI.ShowPopupUI<UILoginPopup>("UILoginPopup");
    }
}
