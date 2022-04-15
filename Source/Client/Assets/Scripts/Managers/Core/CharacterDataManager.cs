using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;

public class CharacterDataManager
{
    List<Dictionary<string, object>> _skill;
    List<Dictionary<string, object>> _level;
    bool _isLoaded = false;

    public void Load()
    {
        if (_isLoaded)
            return;

        _skill = CoreManagers.Data.LoadCSV("Data/Character/CharacterSkill");
        _level = CoreManagers.Data.LoadCSV("Data/Character/CharacterLevel");

        _isLoaded = true;
    }

    public object GetSkill(int skillID, string name)
    {
        return _skill[skillID][name];
    }

    public object GetLevel(int level, string name)
    {
        return _level[level - 1][name];
    }
}
