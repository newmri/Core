using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class UINetworkErrorPopup : UIPopup
{
    enum Buttons
    {
        OKButton
    }

    public override void Init()
    {
        base.Init();

        Bind<Button>(typeof(Buttons));
        GetButton((int)Buttons.OKButton).gameObject.BindEvent(OnClickOKButton);
    }

    public void OnClickOKButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }
}
