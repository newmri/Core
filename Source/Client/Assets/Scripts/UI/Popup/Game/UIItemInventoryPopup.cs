using TMPro;
using Info;
using Define;
using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine.EventSystems;
using System;

public class UIItemInventoryPopup : UIPopup
{
    enum GameObjects
    {
        EquipSlot,
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

    private List<TextMeshProUGUI> _statList = null;

    public byte CurrSlotCunt { get; set; } = 0;
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

        _statList = Util.FindAllChildrens<TextMeshProUGUI>(GetObject((int)GameObjects.Stats), "ValueText", true);

        gameObject.SetActive(false);
    }

    public void Open()
    {
        gameObject.SetActive(true);

        UpdateMoney(Managers.Account.Money);
        UpdateCharacterLevel(Managers.Object.MyPlayer.Level);
        UpdateEXPBar((float)Managers.Object.MyPlayer.EXP / Managers.Object.MyPlayer.MaxEXP);
        UpdateStats();
        OnDeselectedItemSlot();
        var gear = Managers.Object.MyPlayer.Gear;
        for (GearType gearType = 0; gearType < GearType.HAIR; ++gearType)
        {
            _equipList[(int)gearType].ItemSlotInfo = new ItemSlotInfoT { ItemUid = 0, ItemId = gear.Index[(int)gearType], ItemCount = 1 };
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

    private void UpdateStats()
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

    public void UpdateItemSlot(byte maxSlotCount, List<ItemSlotInfoT> itemSlotInfoList)
    {
        GameObject itemSlotContents = GetObject((int)GameObjects.ItemSlotContents);

        foreach (Transform child in itemSlotContents.transform)
            Destroy(child.gameObject);

        for (int i = 0; i < MaxSlotCount; ++i)
        {
            GameObject itemSlot = CoreManagers.Resource.Instantiate("UI/Popup/UIItemSlot", itemSlotContents.transform);
            UIItemSlot uiItemSlot = itemSlot.GetComponent<UIItemSlot>();

            _inventoryList.Add(uiItemSlot);
        }

        CurrSlotCunt = (byte)itemSlotInfoList.Count;
        MaxSlotCount = maxSlotCount;
        this.GetTextMesh((int)TextMeshProUGUIs.ItemSlotCountText).text = $"{CurrSlotCunt} / {MaxSlotCount}";
    }

    public void OnClickBackButton(PointerEventData evt)
    {
        gameObject.SetActive(false);
    }

    public void OnClickItemSlotButton(PointerEventData evt)
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
}
