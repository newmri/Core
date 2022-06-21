using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;
using Define;

public class EffectDataManager
{
    bool _isLoaded = false;

    List<Dictionary<string, object>> _effect;

    public void Load()
    {
        if (_isLoaded)
            return;

        _isLoaded = true;

        CoreManagers.Data.LoadCSV("Data/Effect/Effect", out _effect);
    }
}
