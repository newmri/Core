using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using System;

public static class UIExtension
{
    public static void Init(this UIBase uiBase)
    {

    }

    public static TextMeshProUGUI GetTextMesh(this UIBase uiBase, int index)
    {
        return uiBase.Get<TextMeshProUGUI>(index);
    }

    public static TMP_InputField GetInputText(this UIBase uiBase, int index)
    {
        return uiBase.Get<TMP_InputField>(index);
    }

    public static void BindEvent(this GameObject go, Action<PointerEventData> action, CoreDefine.UIEvent type = CoreDefine.UIEvent.Click)
    {
        UIBase.BindEvent(go, action, type);
    }
}

