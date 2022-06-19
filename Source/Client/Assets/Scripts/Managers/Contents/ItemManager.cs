using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;
using Define;

public class ItemManager
{
    bool _isLoaded = false;

    List<Dictionary<string, object>> _item;
    List<Dictionary<string, object>> _itemStorage;

    public void Load()
    {
        if (_isLoaded)
            return;

        _isLoaded = true;

        _item = CoreManagers.Data.LoadCSV("Data/Item/Item");
        _itemStorage = CoreManagers.Data.LoadCSV("Data/Item/ItemStorage");
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
