using GamePacket;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using Define;
using UnityCoreLibrary;
using System;

public class UIItemInfoPopup : UIPopup
{
    [SerializeField]
    private ItemSlot _itemSlot;
    public ItemSlot ItemSlot
    {
        get
        {
            return _itemSlot;
        }
        set
        {
            _itemSlot = value;
        }
    }

    enum Images
    {
        ItemIcon
    }

    enum TextMeshProUGUIs
    {
        ItemNameText,
        ExplainText,
        LevelText,
        GradeNameText,
        JobNameText
    }

    enum Buttons
    {
        CloseButton
    }

    public override void Init()
    {
        base.Init();

        Bind<Image>(typeof(Images));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
        Bind<Button>(typeof(Buttons));

        GetImage((int)Images.ItemIcon).sprite = Managers.ItemData.GetIcon(ItemSlot.ItemID);

        this.GetTextMesh((int)TextMeshProUGUIs.ItemNameText).text = Managers.ItemData.GetData(ItemSlot.ItemID, "Name").ToString();
        this.GetTextMesh((int)TextMeshProUGUIs.ItemNameText).color = Managers.ItemData.GetColor(ItemSlot.ItemID);

        this.GetTextMesh((int)TextMeshProUGUIs.ExplainText).text = Managers.ItemData.GetData(ItemSlot.ItemID, "Explain").ToString();

        byte minLevel = (byte)(int)Managers.ItemData.GetData(ItemSlot.ItemID, "UsableMinLevel");
        byte maxLevel = (byte)(int)Managers.ItemData.GetData(ItemSlot.ItemID, "UsableMaxLevel");
        this.GetTextMesh((int)TextMeshProUGUIs.LevelText).text = $"LV.{minLevel} ~ {maxLevel}";

        this.GetTextMesh((int)TextMeshProUGUIs.GradeNameText).text = Managers.ItemData.GetGradeName(ItemSlot.ItemID);
        this.GetTextMesh((int)TextMeshProUGUIs.GradeNameText).color = Managers.ItemData.GetColor(ItemSlot.ItemID);

        this.GetTextMesh((int)TextMeshProUGUIs.JobNameText).text = Managers.ItemData.GetJobName(ItemSlot.ItemID);
        this.GetTextMesh((int)TextMeshProUGUIs.JobNameText).color =
            Managers.Object.MyPlayer.Job == (Job)(int)Managers.ItemData.GetData(ItemSlot.ItemID, "Job") ? Color.green : Color.red;

        GetButton((int)Buttons.CloseButton).gameObject.BindEvent(OnClickCloseButton);
    }

    private void OnClickCloseButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }
}
