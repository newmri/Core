using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using Define;

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

        Get<GameObject>((int)GameObjects.ID).GetComponent<TMP_InputField>().characterLimit = (int)AccountLimit.MaxIDLen;
        Get<GameObject>((int)GameObjects.Password).GetComponent<TMP_InputField>().characterLimit = (int)AccountLimit.MaxPasswordLen;
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

        if (!LimiDefine.IsValidAccountLen(id, password))
            return;

        Managers.LoginNetwork.ID = id;
        Managers.LoginNetwork.Password = password;
        Managers.LoginNetwork.WorldListServerLogin();
        ClearText();
    }

    public void ClearText()
    {
        Get<GameObject>((int)GameObjects.ID).GetComponent<TMP_InputField>().text = "";
        Get<GameObject>((int)GameObjects.Password).GetComponent<TMP_InputField>().text = "";
    }
}
