using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;
using Define;
using System;

[Serializable]
public struct EffectData
{
    public string EffectName;
    public EffectType EffectType;
    public int EffectValue;
    public long EffectTick;
    public long EffectDuration;
}

public class EffectDataManager
{
    bool _isLoaded = false;

    Dictionary<int, Dictionary<string, object>> _effect;

    public void Load()
    {
        if (_isLoaded)
            return;

        _isLoaded = true;

        CoreManagers.Data.LoadCSV("EffectID", "Data/Effect/Effect", out _effect);
    }

    public void GetEffect(int effectID, ref EffectData effect)
    {
        if (effectID == 0)
            return;

        effect.EffectName = _effect[effectID]["EffectName"].ToString();
        effect.EffectType = (EffectType)(int)_effect[effectID]["EffectType"];
        effect.EffectValue = (int)_effect[effectID]["EffectValue"];
        effect.EffectTick = (int)_effect[effectID]["EffectTick"];
        effect.EffectDuration = (int)_effect[effectID]["EffectDuration"];
    }
}
