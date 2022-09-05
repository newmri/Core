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
using FlatBuffers;

public class UIItemInfoPopup : UIPopup
{
    [SerializeField]
    private UIItemSlot _itemSlot;
    public UIItemSlot ItemSlot
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
        UsableJobText,
        JobNameText,
        FuctionText
    }

    enum Buttons
    {
        FunctionButton,
        CloseButton
    }

    enum GameObjects
    {
        ItemAbility
    }

    public override void Init()
    {
        base.Init();

        Bind<Image>(typeof(Images));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
        Bind<Button>(typeof(Buttons));
        Bind<GameObject>(typeof(GameObjects));

        GetImage((int)Images.ItemIcon).sprite = Managers.ItemData.GetIcon(ItemSlot.ItemID);
        SetText();
        SetFunction();

        GetButton((int)Buttons.CloseButton).gameObject.BindEvent(OnClickCloseButton);
    }

    private void SetText()
    {
        this.GetTextMesh((int)TextMeshProUGUIs.ItemNameText).text = Managers.ItemData.GetData(ItemSlot.ItemID, "Name").ToString();
        this.GetTextMesh((int)TextMeshProUGUIs.ItemNameText).color = Managers.ItemData.GetColor(ItemSlot.ItemID);

        string explainMessage = Managers.ItemData.GetData(ItemSlot.ItemID, "Explain").ToString().Replace("\\n", "\n");
        for (int i = 0; i < (int)ItemEffect.MAX_COUNT; ++i)
        {
            if(ItemSlot.Effect[i].EffectValue != 0)
            {
                explainMessage = string.Format(explainMessage, ItemSlot.Effect[i].EffectValue);
            }
        }

        this.GetTextMesh((int)TextMeshProUGUIs.ExplainText).text = explainMessage;

        this.GetTextMesh((int)TextMeshProUGUIs.GradeNameText).text = Managers.ItemData.GetGradeName(ItemSlot.ItemID);
        this.GetTextMesh((int)TextMeshProUGUIs.GradeNameText).color = Managers.ItemData.GetColor(ItemSlot.ItemID);

        this.GetTextMesh((int)TextMeshProUGUIs.UsableJobText).text = Managers.ItemData.CanEquip(ItemSlot.ItemID) == true ? "장착 가능:" : "사용 가능:";
        this.GetTextMesh((int)TextMeshProUGUIs.JobNameText).text = Managers.ItemData.GetJobName(ItemSlot.ItemID);
        this.GetTextMesh((int)TextMeshProUGUIs.JobNameText).color = Managers.ItemData.IsUsableJob(ItemSlot.ItemID) ? Color.green : Color.red;

        List<TextMeshProUGUI> abilityNameList = Util.FindAllChildrens<TextMeshProUGUI>(GetObject((int)GameObjects.ItemAbility), "NameText", true);
        List<TextMeshProUGUI> abilityValueList = Util.FindAllChildrens<TextMeshProUGUI>(GetObject((int)GameObjects.ItemAbility), "ValueText", true);
        for(int i = 0; i < abilityNameList.Count; ++i)
        {
            if (ItemSlot.Ability[i].AbilityValue > 0)
            {
                abilityNameList[i].text = ItemSlot.Ability[i].AbilityName;
                abilityValueList[i].text = "+" + ItemSlot.Ability[i].AbilityValue.ToString();
            }
            else
            {
                abilityNameList[i].transform.parent.gameObject.SetActive(false);
            }
        }
    }

    private void SetFunction()
    {
        switch (ItemSlot.ItemLocation)
        {
            case ItemLocation.INVENTORY:
                if(Managers.ItemData.CanEquip(ItemSlot.ItemID))
                {
                    this.GetTextMesh((int)TextMeshProUGUIs.FuctionText).text = "장착";
                    GetButton((int)Buttons.FunctionButton).gameObject.BindEvent(OnClickEquipButton);
                }
                else if(Managers.ItemData.CanUse(ItemSlot.ItemID))
                {
                    this.GetTextMesh((int)TextMeshProUGUIs.FuctionText).text = "사용";
                    GetButton((int)Buttons.FunctionButton).gameObject.BindEvent(OnClickUseButton);
                }
                else
                {
                    GetButton((int)Buttons.FunctionButton).gameObject.SetActive(false);
                }
                break;
            case ItemLocation.EQUIP:
                this.GetTextMesh((int)TextMeshProUGUIs.FuctionText).text = "장착해제";
                GetButton((int)Buttons.FunctionButton).gameObject.BindEvent(OnClickUnEquipButton);
                break;
            default:
                break;
        }
    }

    private void OnClickEquipButton(PointerEventData evt)
    {
        FlatBufferBuilder builder = new FlatBufferBuilder(1);
        var message = CS_EQUIP_GEAR_REQ.CreateCS_EQUIP_GEAR_REQ(builder, ItemSlot.ItemUID);
        Managers.GameNetwork.Send(builder, Packet.CS_EQUIP_GEAR_REQ, message.Value);

        Managers.UI.ClosePopupUI();
    }

    private void OnClickUnEquipButton(PointerEventData evt)
    {
        if(!Managers.UI.GetSceneUI<UIGameScene>().Inventory.HasEmptySlot(1))
        {
            UIErrorMessagePopup errorMessagePopup = Managers.UI.ShowPopupUI<UIErrorMessagePopup>();
            errorMessagePopup.SetText("인벤토리 공간이 부족합니다.");
            return;
        }

        GearType gearType = Util.PascalStringToEnum<GearType>(ItemSlot.gameObject.name);
        FlatBufferBuilder builder = new FlatBufferBuilder(1);
        var message = CS_UNEQUIP_GEAR_REQ.CreateCS_UNEQUIP_GEAR_REQ(builder, gearType);
        Managers.GameNetwork.Send(builder, Packet.CS_UNEQUIP_GEAR_REQ, message.Value);

        Managers.UI.ClosePopupUI();
    }

    private void OnClickUseButton(PointerEventData evt)
    {
        ItemUseError error = ItemSlot.CanUse();
        switch (error)
        {
            case ItemUseError.SUCCESS:
                break;
            case ItemUseError.UNKNOWN:
                {
                    UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();
                    messagePopup.SetText("아이템 사용 실패", "알수 없는 에러입니다");
                    return;
                }
            case ItemUseError.HP_MAX:
                {
                    UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();
                    messagePopup.SetText("아이템 사용 실패", "더 이상 회복 할 수 없습니다");
                    return;
                }
        }

        FlatBufferBuilder builder = new FlatBufferBuilder(1);
        var message = CS_USE_ITEM_REQ.CreateCS_USE_ITEM_REQ(builder, ItemSlot.ItemUID);
        Managers.GameNetwork.Send(builder, Packet.CS_USE_ITEM_REQ, message.Value);

        Managers.UI.ClosePopupUI();
    }

    private void OnClickCloseButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }
}
