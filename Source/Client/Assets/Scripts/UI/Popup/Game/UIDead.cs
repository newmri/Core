using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class UIDead : UIPopup
{
    private int _reviveTime = 5;

    enum TextMeshProUGUIs
    {
        ReviveText
    }

    public override void Init()
    {
        base.Init();
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
        InvokeRepeating("UpdateReviveTime", 1f, 1f);
    }

    private void UpdateReviveTime()
    {
        if(0 == --_reviveTime)
        {
            this.GetTextMesh((int)TextMeshProUGUIs.ReviveText).text = "∫Œ»∞";
            CancelInvoke();
            return;
        }

        this.GetTextMesh((int)TextMeshProUGUIs.ReviveText).text = _reviveTime.ToString();
    }
}
