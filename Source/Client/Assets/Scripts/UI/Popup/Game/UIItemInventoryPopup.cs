using TMPro;
using Info;
using Define;
using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine.EventSystems;
using System;
using System.Collections;
using FlatBuffers;
using GamePacket;

public class UIItemInventoryPopup : UIPopup
{
    enum GameObjects
    {
        EquipSlot,
        StatPoint,
        Stats,
        ItemSlotContents
    }

    enum TextMeshProUGUIs
    {
        CharacterNameText,
        CharacterLevelText,
        EXPText,
        GemText,
        GoldText,
        ItemSlotCountText,
    }

    enum Sliders
    {
        EXPSlider
    }

    enum Buttons
    {
        BackButton
    }

    private UIItemSlot _selectedItemSlot = null;
    private UIItemInfoPopup _selectedItemInfo = null;
    private List<UIItemSlot> _equipList = null;
    private List<UIItemSlot> _inventoryList = new List<UIItemSlot>();

    private List<TextMeshProUGUI> _statPointList = null;
    private List<TextMeshProUGUI> _statList = null;

    byte _currSlotCount = 0;
    public byte CurrSlotCount
    {
        get
        {
            return _currSlotCount;
        }
        set
        {
            _currSlotCount = value;
            this.GetTextMesh((int)TextMeshProUGUIs.ItemSlotCountText).text = $"{CurrSlotCount} / {MaxSlotCount}";
        }
    }

    public byte MaxSlotCount { get; set; } = 50;

    public override void Init()
    {
        base.Init();

        Bind<GameObject>(typeof(GameObjects));
        Bind<Slider>(typeof(Sliders));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.BackButton).gameObject.BindEvent(OnClickBackButton);

        UpdateHandsIcon();

        List<Button> equipButtonList = Util.FindAllChildrens<Button>(GetObject((int)GameObjects.EquipSlot), null, true);
        foreach (var button in equipButtonList)
            button.gameObject.BindEvent(OnClickItemSlotButton);

        List<Button> statPointAddButtonList = Util.FindAllChildrens<Button>(GetObject((int)GameObjects.StatPoint), null, true);
        foreach (var button in statPointAddButtonList)
            button.gameObject.BindEvent(OnClickStatAddButton);

        _statPointList = Util.FindAllChildrens<TextMeshProUGUI>(GetObject((int)GameObjects.StatPoint), "ValueText", true);

        _statList = Util.FindAllChildrens<TextMeshProUGUI>(GetObject((int)GameObjects.Stats), "ValueText", true);

        gameObject.SetActive(false);
    }

    public void Open()
    {
        gameObject.SetActive(true);

        UpdateMoney(Managers.Account.Money);
        UpdateCharacterLevel(Managers.Object.MyPlayer.Level);
        UpdateEXPBar((float)Managers.Object.MyPlayer.EXP / Managers.Object.MyPlayer.MaxEXP);
        UpdateStatPoint();
        UpdateBonusStat();
        UpdateAbility();
        OnDeselectedItemSlot();

        var gear = Managers.Object.MyPlayer.Gear;
        for (GearType gearType = 0; gearType < GearType.HAIR; ++gearType)
        {
            _equipList[(int)gearType].ItemSlotInfo = new ItemSlotInfoT { ItemUid = gear.Info[(int)gearType].ItemUid, ItemId = gear.Info[(int)gearType].ItemId, ItemCount = 1 };
        }
    }

    public void Close()
    {
        gameObject.SetActive(false);
        if (_selectedItemInfo != null)
            _selectedItemInfo.ClosePopupUI();
    }

    public void UpdateMoney(MoneyT money)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.GemText).text = money.Value[(int)MoneyType.GEM].ToString();
        this.GetTextMesh((int)TextMeshProUGUIs.GoldText).text = money.Value[(int)MoneyType.GOLD].ToString();
    }

    public void UpdateCharacterName(string name)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterNameText).text = name;
    }

    public void UpdateCharacterLevel(byte level)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterLevelText).text = level.ToString();
    }

    public void UpdateEXPBar(float value)
    {
       this.GetTextMesh((int)TextMeshProUGUIs.EXPText).text = value + "%";
       GetSlider((int)Sliders.EXPSlider).value = value;
    }

    private void UpdateHandsIcon()
    {
        _equipList = Util.FindAllChildrens<UIItemSlot>(GetObject((int)GameObjects.EquipSlot));

        Sprite leftHandIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{Managers.Object.MyCharacterInfo.Job.ToString()}LeftHandIcon");
        _equipList[(int)GearType.LEFT_HAND].ItemIcon.sprite = leftHandIcon;
        _equipList[(int)GearType.LEFT_HAND].EmptyItemIconSprite = leftHandIcon;

        Sprite rightHandIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{Managers.Object.MyCharacterInfo.Job.ToString()}RightHandIcon");
        _equipList[(int)GearType.RIGHT_HAND].ItemIcon.sprite = rightHandIcon;
        _equipList[(int)GearType.RIGHT_HAND].EmptyItemIconSprite = rightHandIcon;
    }

    private void UpdateStatPoint()
    {
        int index = 0;
        for (StatType statType = 0; statType < StatType.END; ++statType)
        {
            index = (int)statType;

            _statPointList[index].text = Managers.Object.MyCreatureInfo.Stat.Value[index].ToString();
        }

    }

    public void UpdateStatPoint(StatType statType)
    {
        int index = (int)statType;
        _statPointList[index].text = Managers.Object.MyCreatureInfo.Stat.Value[index].ToString();
    }

    public void UpdateBonusStat()
    {
        _statPointList[(int)StatType.END].text = Managers.Object.MyCharacterInfo.BonusStat.ToString();
    }

    public void UpdateAbility()
    {
        int index = 0;
        for (AbilityType abilityType = 0; abilityType < AbilityType.END; ++abilityType)
        {
            index = (int)abilityType;

            if (abilityType.ToString().Contains("RATE"))
                _statList[index].text = ((Managers.Object.MyCreatureInfo.Ability.Value[index] / (float)AbilityRate.RATE) * 100).ToString() + "%";
            else
                _statList[index].text = Managers.Object.MyCreatureInfo.Ability.Value[index].ToString();
        }
    }

    public bool HasEmptySlot(byte count)
    {
        return (MaxSlotCount - CurrSlotCount) >= count;
    }

    public void UpdateItemSlot(byte maxSlotCount, List<ItemSlotInfoT> itemSlotInfoList)
    {
        GameObject itemSlotContents = GetObject((int)GameObjects.ItemSlotContents);

        foreach (Transform child in itemSlotContents.transform)
            Destroy(child.gameObject);

        for (int i = 0; i < MaxSlotCount; ++i)
        {
            GameObject itemSlot = CoreManagers.Resource.Instantiate("UI/Popup/UIItemSlot", itemSlotContents.transform);
            itemSlot.GetComponent<Button>().gameObject.BindEvent(OnClickItemSlotButton);

            UIItemSlot uiItemSlot = itemSlot.GetComponent<UIItemSlot>();
            uiItemSlot.ItemLocation = ItemLocation.INVENTORY;

            _inventoryList.Add(uiItemSlot);
        }

        MaxSlotCount = maxSlotCount;
        CurrSlotCount = 0;

        if (itemSlotInfoList.Count > 0)
            CoreManagers.Coroutine.Add(AddItemDelay(itemSlotInfoList));
    }

    private IEnumerator AddItemDelay(List<ItemSlotInfoT> itemSlotInfoList)
    {
        yield return new WaitUntil(() => _inventoryList[0].IsInited == true);

        foreach (var itemSlotInfo in itemSlotInfoList)
            AddItem(itemSlotInfo);
    }

    public void AddItem(ItemSlotInfoT itemSlotInfo)
    {
        UIItemSlot uiItemSlot = _inventoryList.Find(info => info.ItemID == 0);
        if (uiItemSlot == null)
            return;

        uiItemSlot.ItemSlotInfo = itemSlotInfo;
        ++CurrSlotCount;
    }

    public void RemoveItem(UIItemSlot uiItemSlot, ushort count)
    {
        bool isEmpty = true;

        if (Managers.ItemData.IsStackItem(uiItemSlot.ItemSlotInfo.ItemId))
        {
            uiItemSlot.ItemCount -= count;
            if (uiItemSlot.ItemCount != 0)
                isEmpty = false;
        }

       if(isEmpty)
        {
            uiItemSlot.ItemID = 0;
            --CurrSlotCount;
        }
    }

    public void EquipGear(long itemUID)
    {
        UIItemSlot uiItemSlot = _inventoryList.Find(info => info.ItemUID == itemUID);
        if (uiItemSlot == null)
            return;

        ItemSlotInfoT itemSlotInfo = new ItemSlotInfoT
        {
            ItemUid = uiItemSlot.ItemUID,
            ItemId = uiItemSlot.ItemID,
            ItemCount = uiItemSlot.ItemCount
        };

        RemoveItem(uiItemSlot, 1);

        GearType gearType = Managers.ItemData.GetGearType(itemSlotInfo.ItemId);
        Managers.Object.MyPlayer.SetGear(gearType, itemSlotInfo.ItemUid, itemSlotInfo.ItemId);

        _equipList[(int)gearType].ItemSlotInfo = itemSlotInfo;
    }

    public void UnEquipGear(GearType gearType)
    {
        ItemSlotInfoT itemSlotInfo = new ItemSlotInfoT
        {
            ItemUid = _equipList[(int)gearType].ItemSlotInfo.ItemUid,
            ItemId = _equipList[(int)gearType].ItemSlotInfo.ItemId,
            ItemCount = _equipList[(int)gearType].ItemSlotInfo.ItemCount
        };

        AddItem(itemSlotInfo);

        Managers.Object.MyPlayer.SetGear(gearType, 0, 0);

         _equipList[(int)gearType].ItemID = 0;
    }

    public void OnClickBackButton(PointerEventData evt)
    {
        gameObject.SetActive(false);
    }

    private void OnClickItemSlotButton(PointerEventData evt)
    {
        OnDeselectedItemSlot();

        _selectedItemSlot = evt.selectedObject.GetComponent<UIItemSlot>();
        _selectedItemSlot.OnSelected();

        if (_selectedItemSlot.ItemID > 0)
        {
            _selectedItemInfo = Managers.UI.ShowPopupUI<UIItemInfoPopup>();
            _selectedItemInfo.ItemSlot = _selectedItemSlot;
        }
    }

    private void OnDeselectedItemSlot()
    {
        if (_selectedItemSlot != null)
        {
            _selectedItemSlot.OnDeselected();
            _selectedItemSlot = null;
        }
    }

    private void OnClickStatAddButton(PointerEventData evt)
    {
        StatType statType = (StatType)evt.selectedObject.transform.parent.GetSiblingIndex();
        if(statType >= StatType.END)
        {
            Debug.Log($"StatAddButton Failed order error {statType}");
            return;
        }

        if (Managers.Object.MyCharacterInfo.BonusStat <= 0)
            return;

        FlatBufferBuilder builder = new FlatBufferBuilder(1);
        var message = CS_ADD_STAT_REQ.CreateCS_ADD_STAT_REQ(builder, statType);
        Managers.GameNetwork.Send(builder, Packet.CS_ADD_STAT_REQ, message.Value);
    }
}
