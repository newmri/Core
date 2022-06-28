using System;
using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.UI;
using Define;
using TMPro;
using UnityCoreLibrary.UI;
using Info;

public enum ItemLocation : byte
{
    INVENTORY = 0,
    EQUIP = 1,
    END = 2
};

public class UIItemSlot : UIAwakeBase
{
    [SerializeField]
    ItemSlotInfoT _itemSlotInfo = new ItemSlotInfoT();
    public ItemSlotInfoT ItemSlotInfo
    {
        get
        {
            return _itemSlotInfo;
        }
        set
        {
            _itemSlotInfo = value;
            if(ItemID > 0)
            {
                _frameIcon.sprite = Managers.ItemData.GetFrameIcon(ItemID);

                ItemIcon.sprite = Managers.ItemData.GetIcon(ItemID);
                ItemIcon.color = Color.white;

                Ability = Managers.ItemData.GetAbility(ItemID);
            }
            else
            {
                _frameIcon.sprite = _emptyFrameIconSprite;

                ItemIcon.sprite = EmptyItemIconSprite;
                ItemIcon.color = _emptyColor;

                Array.Clear(_itemAbility, 0, (int)ItemAbility.MAX_COUNT);
            }
        }
    }

    public int ItemID
    {
        get
        {
            return ItemSlotInfo.ItemId;
        }
        set
        {
            ItemSlotInfo.ItemId = value;
        }
    }

    [SerializeField]
    ItemLocation _itemLocation;
    public ItemLocation ItemLocation
    {
        get
        {
            return _itemLocation;
        }
        set
        {
            _itemLocation = value;
        }
    }

    [SerializeField]
    Ability[] _itemAbility = new Ability[(int)ItemAbility.MAX_COUNT];
    public Ability[] Ability
    {
        get
        {
            return _itemAbility;
        }
        set
        {
            _itemAbility = value;
        }
    }

    private Image _frameIcon = null;
    private Sprite _emptyFrameIconSprite = null;
    private Image _selectIcon = null;

    public Image ItemIcon = null;
    public Sprite EmptyItemIconSprite = null;

    private Color _emptyColor = new Color(0.373f, 0.306f, 0.392f, 1.0f);

    private TextMeshProUGUI _countText = null;

    public override void Init()
    {
        _frameIcon = Util.GetOrAddComponent<Image>(gameObject);
        _emptyFrameIconSprite = _frameIcon.sprite;

        ItemIcon = Util.FindChild<Image>(gameObject, "Icon");
        EmptyItemIconSprite = ItemIcon.sprite;

        _selectIcon = Util.FindChild<Image>(gameObject, "SelectIcon");

        if (ItemLocation != ItemLocation.EQUIP)
            _countText = Util.FindChild<TextMeshProUGUI>(gameObject, "CountText");
    }

    public void OnSelected()
    {
        _selectIcon.gameObject.SetActive(true);
    }

    public void OnDeselected()
    {
        _selectIcon.gameObject.SetActive(false);
    }
}
