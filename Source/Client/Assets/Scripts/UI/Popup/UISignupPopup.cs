using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class UISignupPopup : UIPopup
{
    enum GameObjects
    {
        AccountName,
        Password
    }

    enum Buttons
    {
        SignupButton,
        CloseButton
    }

    public override void Init()
    {
        base.Init();

        Bind<GameObject>(typeof(GameObjects));
        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.SignupButton).gameObject.BindEvent(OnClickSignupButton);
        GetButton((int)Buttons.CloseButton).gameObject.BindEvent(OnClickCloseButton);
    }

    [System.Obsolete]
    public void OnClickSignupButton(PointerEventData evt)
    {
        string account = Get<GameObject>((int)GameObjects.AccountName).GetComponent<TMP_InputField>().text;
        string password = Get<GameObject>((int)GameObjects.Password).GetComponent<TMP_InputField>().text;

        SignupAccountPacketReq packet = new SignupAccountPacketReq()
        {
            AccountName = account,
            Password = password
        };

        Managers.Web.SendPostRequest<SignupAccountPacketRes>("signup", packet, (res) =>
        {
            if (res.IsSuccess)
            {
                Managers.UI.ClosePopupUI(true);
                UIMessagePopup p = Managers.UI.ShowPopupUI<UIMessagePopup>();
                p.SetText("Welcome to our world!", "Mr." + account);
            }
        });
    }

    public void OnClickCloseButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI(true);
    }
}
