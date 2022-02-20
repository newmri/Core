using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class UIMessagePopup : UIPopup
{
    enum GameObjects
    {
        TitleText,
        MessageText,
        OKButton
    }

    private string _title;
    private string _message;

    public override void Init()
    {
        base.Init();

        Bind<GameObject>(typeof(GameObjects));
        Get<GameObject>((int)GameObjects.OKButton).gameObject.BindEvent(OnClickOKButton);
        Get<GameObject>((int)GameObjects.TitleText).GetComponent<TextMeshProUGUI>().text = _title;
        Get<GameObject>((int)GameObjects.MessageText).GetComponent<TextMeshProUGUI>().text = _message;
    }

    public void SetText(string title, string message)
    {
        _title = title;
        _message = message;
    }

    public void OnClickOKButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }
}
