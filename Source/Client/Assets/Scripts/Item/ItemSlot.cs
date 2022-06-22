using System;
using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.UI;

public class ItemSlot : MonoBehaviour
{
    [SerializeField]
    int _itemID;
    public int ItemID
    {
        get
        {
            return _itemID;
        }
        set
        {
            _itemID = value;
            if(_itemID > 0)
            {
                _frameIcon.sprite = Managers.ItemData.GetFrameIcon(_itemID);

                ItemIcon.sprite = Managers.ItemData.GetIcon(_itemID);
                ItemIcon.color = Color.white;
            }
            else
            {
                _frameIcon.sprite = _emptyFrameIconSprite;

                ItemIcon.sprite = EmptyItemIconSprite;
                ItemIcon.color = _emptyColor;
            }
        }
    }

    private Image _frameIcon = null;
    private Sprite _emptyFrameIconSprite = null;

    public Image ItemIcon = null;
    public Sprite EmptyItemIconSprite = null;

    private Image _selectIcon = null;

    private Color _emptyColor = new Color(0.373f, 0.306f, 0.392f, 1.0f);

    // Start is called before the first frame update
    void Awake()
    {
        _frameIcon = Util.GetOrAddComponent<Image>(gameObject);
        _emptyFrameIconSprite = _frameIcon.sprite;

        ItemIcon = Util.FindChild<Image>(gameObject, "Icon");
        EmptyItemIconSprite = ItemIcon.sprite;

        _selectIcon = Util.FindChild<Image>(gameObject, "SelectIcon");
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
