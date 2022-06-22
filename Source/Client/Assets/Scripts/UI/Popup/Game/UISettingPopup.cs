using GamePacket;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using Define;
using UnityCoreLibrary;
using System.Threading;

public class UISettingPopup : UIPopup
{
    enum Buttons
    {
        CloseButton,
        SelectCharacterButton,
        QuitGameButton
    }

    public override void Init()
    {
        base.Init();
        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.CloseButton).gameObject.BindEvent(OnClickCloseButton);
        GetButton((int)Buttons.SelectCharacterButton).gameObject.BindEvent(OnClickSelectCharacterButton);
        GetButton((int)Buttons.QuitGameButton).gameObject.BindEvent(OnQuitGameButton);
    }

    public void OnClickCloseButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }

    public void OnClickSelectCharacterButton(PointerEventData evt)
    {
        Managers.GameNetwork.Disconnect();

        Managers.Clear();

        CoreManagers.Scene.Clear();
        CoreManagers.Scene.LoadScene(CoreDefine.Scene.Login, Managers.LoginNetwork.Conntect);
    }

    public void OnQuitGameButton(PointerEventData evt)
    {
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#else
        Application.Quit(); // 어플리케이션 종료
#endif
    }
}
