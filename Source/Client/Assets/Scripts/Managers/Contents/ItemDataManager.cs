using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;
using Define;
using Info;
using System;

public class ItemDataManager
{
    bool _isLoaded = false;

    Dictionary<int, Dictionary<string, object>> _item;
    List<Dictionary<string, object>> _itemStorage;

    public void Load()
    {
        if (_isLoaded)
            return;

        _isLoaded = true;

        CoreManagers.Data.LoadCSV("ItemID", "Data/Item/Item", out _item);
        CoreManagers.Data.LoadCSV("Data/Item/ItemStorage", out _itemStorage);
    }

    public Sprite GetFrameIcon(int itemID)
    {
       GradeType gradeType = (GradeType)(int)GetData(itemID, "Grade");
       return Managers.GradeData.GradeInfoList[(int)gradeType].frame;
    }

    public Sprite GetIcon(int itemID)
    {
        return CoreManagers.Resource.Load<Sprite>(GetData(itemID, "IconPath").ToString());
    }

    public Color GetColor(int itemID)
    {
        GradeType gradeType = (GradeType)(int)GetData(itemID, "Grade");
        return Managers.GradeData.GradeInfoList[(int)gradeType].color;
    }

    public string GetGradeName(int itemID)
    {
        GradeType gradeType = (GradeType)(int)GetData(itemID, "Grade");
        return Managers.GradeData.GradeInfoList[(int)gradeType].name;
    }

    public string GetJobName(int itemID)
    {
        return Managers.LoginData.GetJobExplain((Job)(int)GetData(itemID, "Job"), "Name").ToString();
    }

    // ItemID -> GearID 로 변환
    public CharacterGearT ToGearID(CharacterGearT gears)
    {
        CharacterGearT convertedGears = new CharacterGearT();

        for (int i = 0; i < (int)GearType.END; ++i)
            convertedGears.Index[i] = GetGearID(gears.Index[i]);

        return convertedGears;
    }

    // ItemID -> GearID 로 변환
    public int GetGearID(int itemID)
    {
        if (itemID == 0)
            return 0;

        return itemID % (int)GetItemType(itemID);
    }

    public bool CanEquip(int itemID)
    {
        switch(GetItemType(itemID))
        {
            case ItemType.MELEE:
            case ItemType.BOW:
            case ItemType.STAFF:
            case ItemType.SHIELD:
            case ItemType.QUIVER:
            case ItemType.ARMOR:
            case ItemType.HELMET:
            case ItemType.SHOULDER:
            case ItemType.ARM:
            case ItemType.FEET:
                break;
            default:
                return false;
        }

        return IsUsableItem(itemID);
    }

    public bool CanUse(int itemID)
    {
        return IsUsableItem(itemID);
    }

    public ItemType GetItemType(int itemID)
    {
        return (ItemType)(int)GetData(itemID, "ItemType");
    }

    public bool IsUsableItem(int itemID)
    {
        return IsUsableJob(itemID);
    }


    public bool IsUsableJob(int itemID)
    {
        Job job = (Job)(int)GetData(itemID, "Job");
        if (job == Job.NONE)
            return true;

        return (job == Managers.Object.MyPlayer.Job);
    }

    public object GetData(int itemID, string name)
    {
        return _item[itemID][name];
    }
}
