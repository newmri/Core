using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class UILoginPopup : UIPopup
{
    enum Buttons
    {
        SignupButton,
        CloseButton
    }

    public override void Init()
    {
        base.Init();

        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.SignupButton).gameObject.BindEvent(OnClickSignupButton);
        GetButton((int)Buttons.CloseButton).gameObject.BindEvent(OnClickCloseButton);
    }

    public void OnClickSignupButton(PointerEventData evt)
    {
        gameObject.SetActive(false);
        CoreManagers.UI.ShowPopupUI<UISignupPopup>("UISignupPopup");
    }

    public void OnClickCloseButton(PointerEventData evt)
    {
        CoreManagers.UI.ClosePopupUI();
    }
}
