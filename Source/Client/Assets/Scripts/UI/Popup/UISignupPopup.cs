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
        ID,
        Password
    }

    enum Buttons
    {
        SignupButton,
        CloseButton,
        LoginButton
    }

    public override void Init()
    {
        base.Init();

        Bind<GameObject>(typeof(GameObjects));
        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.SignupButton).gameObject.BindEvent(OnClickSignupButton);
        GetButton((int)Buttons.CloseButton).gameObject.BindEvent(OnClickCloseButton);
        GetButton((int)Buttons.LoginButton).gameObject.BindEvent(OnClickLoginButton);
    }

    [System.Obsolete]
    public void OnClickSignupButton(PointerEventData evt)
    {
        string id = Get<GameObject>((int)GameObjects.ID).GetComponent<TMP_InputField>().text;
        string password = Get<GameObject>((int)GameObjects.Password).GetComponent<TMP_InputField>().text;

        SignupAccountPacketReq packet = new SignupAccountPacketReq()
        {
            ID = id,
            Password = password
        };

        Managers.Web.SendPostRequest<SignupAccountPacketRes>("signup", packet, (res) =>
        {
            if (res.IsSuccess)
            {
                Managers.UI.ClosePopupUI(true);
                UIMessagePopup p = Managers.UI.ShowPopupUI<UIMessagePopup>();
                p.SetText("환영합니다", "모험가 " + id + "님");
            }
        });
    }

    public void OnClickCloseButton(PointerEventData evt)
    {
        Managers.UI.CloseAllPopupUI();
    }

    public void OnClickLoginButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI(true);
    }
}
