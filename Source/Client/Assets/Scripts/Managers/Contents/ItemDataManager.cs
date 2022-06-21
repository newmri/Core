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

        return itemID % (int)_item[itemID]["ItemType"];
    }

    public Sprite GetGearIcon(Job job, GearType gearType, byte gearIndex)
    {
        string name = GetGearName(job, gearType);
        string path = "UI/Item/Gear/" + Util.EnumToPascal(name) + "/" + name + " " + gearIndex.ToString();

        return CoreManagers.Resource.Load<Sprite>(path);
    }

    private string GetGearName(Job job, GearType gear)
    {
        string name = "";
        if (gear == GearType.LEFT_HAND || gear == GearType.RIGHT_HAND)
        {
            if (job == Job.WARRIOR)
            {
                if (gear == GearType.LEFT_HAND)
                    name = "MELEE";
                else
                    name = "SHIELD";
            }
            else if (job == Job.ARCHER)
            {
                if (gear == GearType.LEFT_HAND)
                    name = "BOW";
                else
                    name = "QUIVER";
            }
            else if (job == Job.SORCERER)
            {
                if (gear == GearType.LEFT_HAND)
                    name = "STAFF";
            }
            else if (job == Job.DUELIST)
            {
                name = "MELEE";
            }
        }
        else
        {
            name = gear.ToString();
        }

        return name;
    }
}
