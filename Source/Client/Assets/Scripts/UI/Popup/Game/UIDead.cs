using GamePacket;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class UIDead : UIPopup
{
    private int _reviveTime = 5;

    enum TextMeshProUGUIs
    {
        ReviveText
    }

    enum Buttons
    {
        ReviveButton
    }

    public override void Init()
    {
        base.Init();
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.ReviveButton).gameObject.BindEvent(OnClickReviveButton);
        InvokeRepeating("UpdateReviveTime", 1f, 1f);
    }

    public void OnClickReviveButton(PointerEventData evt)
    {
        if (0 == _reviveTime)
        {
            Managers.GameNetwork.Send(Packet.CS_REVIVE_REQ);
            Managers.UI.ClosePopupUI();
        }
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
