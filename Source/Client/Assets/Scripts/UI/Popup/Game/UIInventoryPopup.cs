using TMPro;
using Info;
using Define;
using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine.EventSystems;

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

    private List<Image> _equipList = null;
    private List<TextMeshProUGUI> _statList = null;

    private Color _equipEmptyColor = new Color(0.373f, 0.306f, 0.392f, 1.0f);

    public override void Init()
    {
        base.Init();

        Bind<GameObject>(typeof(GameObjects));
        Bind<Slider>(typeof(Sliders));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.BackButton).gameObject.BindEvent(OnClickBackButton);

        UpdateHandsIcon();

        _statList = Util.FindAllChildrens<TextMeshProUGUI>(GetObject((int)GameObjects.Stats), "ValueText", true);

        gameObject.SetActive(false);
    }

    public void Refresh()
    {
        UpdateEquipSlotIcon();
        UpdateMoney(Managers.Account.Money);
        UpdateCharacterLevel(Managers.Object.MyPlayer.Level);
        UpdateEXPBar((float)Managers.Object.MyPlayer.EXP / Managers.Object.MyPlayer.MaxEXP);
        UpdateStats();
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
        _equipList = Util.FindAllChildrens<Image>(GetObject((int)GameObjects.EquipSlot), "Icon", true);
        Sprite leftHandIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{Managers.Object.MyCharacterInfo.Job.ToString()}LeftHandIcon");
        UpdateEquipSlotIcon(GearType.LEFT_HAND, leftHandIcon);

        Sprite rightHandIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{Managers.Object.MyCharacterInfo.Job.ToString()}RightHandIcon");
        UpdateEquipSlotIcon(GearType.RIGHT_HAND, rightHandIcon);
    }

    public void UpdateEquipSlotIcon()
    {
        var gear = Managers.Object.MyPlayer.Gear;
        for (GearType gearType = 0; gearType < GearType.END; ++gearType)
        {
            if(gear.Index[(int)gearType] > 0)
                UpdateEquipSlotIcon(gearType, Managers.ItemData.GetGearIcon(Managers.Object.MyPlayer.Job, gearType, (byte)gear.Index[(int)gearType]), true);
        }
    }

    public void UpdateEquipSlotIcon(GearType gearType, Sprite icon, bool isEquiped = false)
    {
        _equipList[(int)gearType].sprite = icon;

        if (isEquiped)
            _equipList[(int)gearType].color = Color.white;
        else
            _equipList[(int)gearType].color = _equipEmptyColor;
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

    public void OnClickBackButton(PointerEventData evt)
    {
        gameObject.SetActive(false);
    }
}
