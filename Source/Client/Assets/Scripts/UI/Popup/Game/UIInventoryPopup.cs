using TMPro;
using Info;
using Define;
using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine.EventSystems;
using System;

public class UIInventoryPopup : UIPopup
{
    enum GameObjects
    {
        EquipSlot,
        Stats
    }

    enum TextMeshProUGUIs
    {
        CharacterNameText,
        CharacterLevelText,
        EXPText,
        GemText,
        GoldText,
    }

    enum Sliders
    {
        EXPSlider
    }

    enum Buttons
    {
        BackButton
    }

    private ItemSlot _selectedItemSlot = null;
    private UIItemInfoPopup _selectedItemInfo = null;
    private List<ItemSlot> _equipList = null;

    private List<TextMeshProUGUI> _statList = null;

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
        UpdateItemSlot();
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
        _equipList = Util.FindAllChildrens<ItemSlot>(GetObject((int)GameObjects.EquipSlot));

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

    private void UpdateItemSlot()
    {
        OnDeselectedItemSlot();

        var gear = Managers.Object.MyPlayer.Gear;
        for (GearType gearType = 0; gearType < GearType.HAIR; ++gearType)
        {
            _equipList[(int)gearType].ItemID = gear.Index[(int)gearType];
        }
    }

    public void OnClickBackButton(PointerEventData evt)
    {
        gameObject.SetActive(false);
    }

    public void OnClickItemSlotButton(PointerEventData evt)
    {
        OnDeselectedItemSlot();

        _selectedItemSlot = evt.selectedObject.GetComponent<ItemSlot>();
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
