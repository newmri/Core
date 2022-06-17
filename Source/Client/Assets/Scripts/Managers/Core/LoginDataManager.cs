using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;
using Define;
using System.Linq;

public class LoginDataManager
{
    List<Dictionary<string, object>> _createCharacterGear;
    List<Dictionary<string, object>> _JobExplain;
    bool _isLoaded = false;

    public void Load()
    {
        if (_isLoaded)
            return;

        _createCharacterGear = CoreManagers.Data.LoadCSV("Data/Character/CharacterCreateGear");
        _JobExplain = CoreManagers.Data.LoadCSV("Data/Character/JobExplain");

        _isLoaded = true;
    }

    public Info.CharacterGearT GetCreateCharacterGear(Job job)
    {
        Info.CharacterGearT gear = new Info.CharacterGearT();

        for (GearType i = 0; i < GearType.END; ++i)
        {
           gear.Index[(int)i] = (byte)(int)_createCharacterGear[(int)job][Util.EnumToPascal(i.ToString())];
        }

        return gear;
    }

    public object GetJobExplain(Define.Job job, string name)
    {
        return _JobExplain[(int)job][name];
    }
}
