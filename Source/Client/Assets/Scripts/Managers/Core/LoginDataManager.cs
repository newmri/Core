using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;
using Define;
using System.Linq;

public class LoginDataManager
{
    List<Dictionary<string, object>> _createCharacterGear;
    Dictionary<int, Dictionary<string, object>> _JobExplain;

    bool _isLoaded = false;

    public void Load()
    {
        if (_isLoaded)
            return;

        CoreManagers.Data.LoadCSV("Data/Character/CharacterCreateGear", out _createCharacterGear);
        CoreManagers.Data.LoadCSV("Job", "Data/Character/JobExplain", out _JobExplain);

        _isLoaded = true;
    }

    public Info.CharacterGearT GetCreateCharacterGear(Job job)
    {
        Info.CharacterGearT gear = new Info.CharacterGearT();

        for (GearType i = 0; i < GearType.END; ++i)
        {
           gear.Info[(int)i] = new Info.GearSlotInfoT();
           gear.Info[(int)i].ItemId = (int)_createCharacterGear[(int)job][Util.EnumToPascal(i.ToString())];
        }

        return gear;
    }

    public object GetJobExplain(Define.Job job, string name)
    {
        return _JobExplain[(int)job][name];
    }
}
