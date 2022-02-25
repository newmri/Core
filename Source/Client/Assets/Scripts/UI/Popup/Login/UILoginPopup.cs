using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class UILoginPopup : UIPopup
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

    public void OnClickSignupButton(PointerEventData evt)
    {
        gameObject.SetActive(false);
        Managers.UI.ShowPopupUI<UISignupPopup>();
    }

    public void OnClickCloseButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }

    [System.Obsolete]
    public void OnClickLoginButton(PointerEventData evt)
    {
        string id = Get<GameObject>((int)GameObjects.ID).GetComponent<TMP_InputField>().text;
        string password = Get<GameObject>((int)GameObjects.Password).GetComponent<TMP_InputField>().text;

        if (!AccountDefine.IsValidAccount(id, password))
            return;

        LoginAccountPacketReq packet = new LoginAccountPacketReq()
        {
            ID = id,
            Password = password
        };

        Managers.Web.SendPostRequest<LoginAccountPacketRes>("login", packet, (res) =>
        {
            ClearText();


            if (res.IsSuccess)
            {
                Managers.LoginNetwork.UID = res.UID;
                Managers.LoginNetwork.Token = res.Token;
                Managers.UI.ShowPopupUI<UIWorldListPopup>().SetWorldList(res.WorldList);
            }
            else
            {
                UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();
                messagePopup.SetText("로그인 실패", "아이디 또는 비밀번호가 틀렸습니다\n 다시 입력해주세요");
            }
        });
    }

    public void ClearText()
    {
        Get<GameObject>((int)GameObjects.ID).GetComponent<TMP_InputField>().text = "";
        Get<GameObject>((int)GameObjects.Password).GetComponent<TMP_InputField>().text = "";
    }
}
