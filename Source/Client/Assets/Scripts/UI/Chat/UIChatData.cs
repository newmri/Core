using Define;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityCoreLibrary;
using UnityEngine;

public class UIChatData : UIBase
{
    public ChatType ChatType;
    
    public override void Init()
    {

    }

    public void SetChatData(ChatType chatType, Color color, string text)
    {
        ChatType = chatType;

        TextMeshProUGUI textUGUI = GetComponent<TextMeshProUGUI>();
        textUGUI.color = color;
        textUGUI.text = text;
    }

}
