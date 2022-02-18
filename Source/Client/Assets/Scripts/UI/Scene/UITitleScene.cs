using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class UITitleScene : UIScene
{
    enum Buttons
    {
        StartButton
    }

    enum TextMeshProUGUIs
    {
        Text_TapToStart
    }

    public override void Init()
    {
        base.Init();

        Bind<Button>(typeof(Buttons));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));

        GetButton((int)Buttons.StartButton).gameObject.BindEvent(OnEnterButton, Define.UIEvent.Enter);
        GetButton((int)Buttons.StartButton).gameObject.BindEvent(OnExitButton, Define.UIEvent.Exit);
        GetButton((int)Buttons.StartButton).gameObject.BindEvent(OnClickStartButton);
    }

    public void OnEnterButton(PointerEventData evt)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.Text_TapToStart).color = Color.green;
    }

    public void OnExitButton(PointerEventData evt)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.Text_TapToStart).color = Color.white;
    }

    public void OnClickStartButton(PointerEventData evt)
    {
        CoreManagers.Scene.LoadScene(Define.Scene.Login);
    }
}
