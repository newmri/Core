using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using System;

public class UINameMessagePopup : UIPopup
{
    enum GameObjects
    {
        TitleText,
        Name,
        PlaceholderText,
        InfoText,
        OKButton,
        CancelButton
    }

    private string _title;
    private string _placeholder;
    public string Name
    {
        get
        {
            return Get<GameObject>((int)GameObjects.Name).GetComponent<TMP_InputField>().text;
        }

        private set { }
    }
    
    private string _info;
    private Action<PointerEventData> _okAction;

    public override void Init()
    {
        base.Init();

        Bind<GameObject>(typeof(GameObjects));
        Get<GameObject>((int)GameObjects.TitleText).GetComponent<TextMeshProUGUI>().text = _title;
        Get<GameObject>((int)GameObjects.PlaceholderText).GetComponent<TextMeshProUGUI>().text = _placeholder;
        Get<GameObject>((int)GameObjects.InfoText).GetComponent<TextMeshProUGUI>().text = _info;

        Get<GameObject>((int)GameObjects.OKButton).gameObject.BindEvent(_okAction);
        Get<GameObject>((int)GameObjects.CancelButton).gameObject.BindEvent(OnClickCancelButton);
    }

    public void SetText(string title, string placeholder, string info, Action<PointerEventData> okAction)
    {
        _title = title;
        _placeholder = placeholder;
        _info = info;
        _okAction = okAction;
    }

    public void OnClickCancelButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }
}
