using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityCoreLibrary;
using UnityEngine;

public class UIChat : UIBase
{
    enum InputTexts
    {
        InputText
    }

    public override void Init()
    {
        Bind<TMP_InputField>(typeof(InputTexts));
        gameObject.SetActive(false);
    }

    public void OnUpdate()
    {
        GetUIKeyInput();
    }

    public bool IsFocused()
    {
        return this.GetInputText((int)InputTexts.InputText).isFocused;
    }

    private void GetUIKeyInput()
    {
        if (Input.GetKeyDown(KeyCode.Return))
        {
            if (gameObject.activeSelf)
            {
                gameObject.SetActive(false);
            }
            else
            {
                gameObject.SetActive(true);
                this.GetInputText((int)InputTexts.InputText).ActivateInputField();
            }
        }
    }
}
