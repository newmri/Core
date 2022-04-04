using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;

public class CharacterDataManager
{
    List<Dictionary<string, object>> _skill;
    bool _isLoaded = false;

    public void Load()
    {
        if (_isLoaded)
            return;

        _skill = CoreManagers.Data.LoadCSV("Data/Character/CharacterSkill");

        _isLoaded = true;
    }

    public object GetSkill(int skillID, string name)
    {
        return _skill[skillID][name];
    }
}
