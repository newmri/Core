using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class UIErrorMessagePopup : UIPopup
{
    enum GameObjects
    {
        ErrorText
    }

    private string _message;

    public override void Init()
    {
        base.Init();

        Bind<GameObject>(typeof(GameObjects));
        Get<GameObject>((int)GameObjects.ErrorText).GetComponent<TextMeshProUGUI>().text = _message;
    }

    public void SetText(string message, float lifeTime = 1.0f)
    {
        _message = message;
        Invoke("DestroyObject", lifeTime);
    }

    private void DestroyObject()
    {
        Managers.UI.ClosePopupUI();
    }
}
