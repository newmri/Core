using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;

public class CharacterDataManager
{
    bool _isLoaded = false;

    List<Dictionary<string, object>> _skill;
    List<Dictionary<string, object>> _level;

    public void Load()
    {
        if (_isLoaded)
            return;

        _isLoaded = true;

        _skill = CoreManagers.Data.LoadCSV("Data/Character/CharacterSkill");
        _level = CoreManagers.Data.LoadCSV("Data/Character/CharacterLevel");
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
