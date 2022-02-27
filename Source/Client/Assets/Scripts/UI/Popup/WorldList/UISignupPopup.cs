using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using Define;

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

        Get<GameObject>((int)GameObjects.ID).GetComponent<TMP_InputField>().characterLimit = (int)AccountLimit.MaxIDLen;
        Get<GameObject>((int)GameObjects.Password).GetComponent<TMP_InputField>().characterLimit = (int)AccountLimit.MaxPasswordLen;
    }

    [System.Obsolete]
    public void OnClickSignupButton(PointerEventData evt)
    {
        string id = Get<GameObject>((int)GameObjects.ID).GetComponent<TMP_InputField>().text;
        string password = Get<GameObject>((int)GameObjects.Password).GetComponent<TMP_InputField>().text;

        if (!LimiDefine.IsValidAccountLen(id, password))
            return;

        SignupAccountPacketReq packet = new SignupAccountPacketReq()
        {
            ID = id,
            Password = password
        };

        Managers.Web.SendPostRequest<SignupAccountPacketRes>("signup", packet, (res) =>
        {
            ClearText();

            UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();

            if (res.IsSuccess)
            {
                messagePopup.SetText("회원가입 성공", "환영합니다 모험가 " + id + "님");
            }
            else
            {
                messagePopup.SetText("회원가입 실패", "이미 존재하는 아이디입니다\n 다른 아이디를 입력해주세요.");
            }
        });
    }

    public void ClearText()
    {
        Get<GameObject>((int)GameObjects.ID).GetComponent<TMP_InputField>().text = "";
        Get<GameObject>((int)GameObjects.Password).GetComponent<TMP_InputField>().text = "";
    }

    public void OnClickCloseButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI(true);
    }

    public void OnClickLoginButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI(true);
    }
}
