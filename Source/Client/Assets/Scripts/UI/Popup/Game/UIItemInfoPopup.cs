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
        GradeNameText,
        JobNameText,
        FuctionText
    }

    enum Buttons
    {
        FunctionButton,
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

        this.GetTextMesh((int)TextMeshProUGUIs.GradeNameText).text = Managers.ItemData.GetGradeName(ItemSlot.ItemID);
        this.GetTextMesh((int)TextMeshProUGUIs.GradeNameText).color = Managers.ItemData.GetColor(ItemSlot.ItemID);

        this.GetTextMesh((int)TextMeshProUGUIs.JobNameText).text = Managers.ItemData.GetJobName(ItemSlot.ItemID);
        this.GetTextMesh((int)TextMeshProUGUIs.JobNameText).color = Managers.ItemData.IsUsableJob(ItemSlot.ItemID) ? Color.green : Color.red;

        SetFunction();

        GetButton((int)Buttons.CloseButton).gameObject.BindEvent(OnClickCloseButton);
    }

    private void SetFunction()
    {
        switch (ItemSlot.ItemLocation)
        {
            case ItemLocation.INVENTORY:
                if(Managers.ItemData.CanEquip(ItemSlot.ItemID))
                {
                    this.GetTextMesh((int)TextMeshProUGUIs.FuctionText).text = "ÀåÂø";
                    GetButton((int)Buttons.FunctionButton).gameObject.BindEvent(OnClickEquipButton);
                }
                else if(Managers.ItemData.CanUse(ItemSlot.ItemID))
                {
                    this.GetTextMesh((int)TextMeshProUGUIs.FuctionText).text = "»ç¿ë";
                    GetButton((int)Buttons.FunctionButton).gameObject.BindEvent(OnClickUseButton);
                }
                else
                {
                    GetButton((int)Buttons.FunctionButton).gameObject.SetActive(false);
                }
                break;
            case ItemLocation.EQUIP:
                this.GetTextMesh((int)TextMeshProUGUIs.FuctionText).text = "ÀåÂøÇØÁ¦";
                GetButton((int)Buttons.FunctionButton).gameObject.BindEvent(OnClickUnEquipButton);
                break;
            default:
                break;
        }
    }

    private void OnClickEquipButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }

    private void OnClickUnEquipButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }

    private void OnClickUseButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }

    private void OnClickCloseButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }
}
