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
    public bool IsInited = false;

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
            OnUpdate();
        }
    }

    public long ItemUID
    {
        get
        {
            return ItemSlotInfo.ItemUid;
        }
        set
        {
            ItemSlotInfo.ItemUid = value;
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
            OnUpdate();

            if(ItemSlotInfo.ItemId == 0)
            {
                OnDeselected();
            }
        }
    }

    public ushort ItemCount
    {
        get
        {
            return ItemSlotInfo.ItemCount;
        }
        set
        {
            ItemSlotInfo.ItemCount = value;
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

    [SerializeField]
    EffectData[] _itemEffect = new EffectData[(int)ItemEffect.MAX_COUNT];
    public EffectData[] Effect
    {
        get
        {
            return _itemEffect;
        }
        set
        {
            _itemEffect = value;
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

        IsInited = true;
    }

    private void OnUpdate()
    {
        if (ItemID > 0)
        {
            _frameIcon.sprite = Managers.ItemData.GetFrameIcon(ItemID);

            ItemIcon.gameObject.SetActive(true);
            ItemIcon.sprite = Managers.ItemData.GetIcon(ItemID);
            ItemIcon.color = Color.white;

            if(_countText != null)
            {
                if (Managers.ItemData.IsStackItem(ItemID))
                {
                    _countText.gameObject.SetActive(true);
                    _countText.text = ItemCount.ToString();
                }
                else
                    _countText.text = "";
            }

            Ability = Managers.ItemData.GetAbility(ItemID);
            Effect = Managers.ItemData.GetEffect(ItemID);
        }
        else
        {
            ItemUID = 0;

            _frameIcon.sprite = _emptyFrameIconSprite;

            if(ItemLocation != ItemLocation.EQUIP)
                ItemIcon.gameObject.SetActive(false);

            ItemIcon.sprite = EmptyItemIconSprite;
            ItemIcon.color = _emptyColor;

            if (_countText != null)
                _countText.text = "";

            Array.Clear(_itemAbility, 0, (int)ItemAbility.MAX_COUNT);
        }
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
