using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public static class UIExtension
{
    public static void Init(this UIBase uiBase)
    {

    }

    public static TextMeshProUGUI GetTextMsh(this UIBase uiBase, int index)
    {
        return uiBase.Get<TextMeshProUGUI>(index);
    }
}

