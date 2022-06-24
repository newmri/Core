using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;
using Define;

public struct Ability
{
    public string AbilityName;
    public AbilityType AbilityType;
    public int AbilityValue;
}

public class AbilityDataManager
{
    bool _isLoaded = false;

    Dictionary<int, Dictionary<string, object>> _ability;

    public void Load()
    {
        if (_isLoaded)
            return;

        _isLoaded = true;

        CoreManagers.Data.LoadCSV("AbilityID", "Data/Ability/Ability", out _ability);
    }

    public void GetAbility(int abilityID, ref Ability ability)
    {
        if (abilityID == 0)
            return;

        ability.AbilityName = _ability[abilityID]["AbilityName"].ToString();
        ability.AbilityType = (AbilityType)(int)_ability[abilityID]["AbilityType"];
        ability.AbilityValue = (int)_ability[abilityID]["AbilityValue"];
    }
}
