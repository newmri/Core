using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;

public class LoginDataManager
{
    List<Dictionary<string, object>> _JobExplain;
    bool _isLoaded = false;

    public void Load()
    {
        if (_isLoaded)
            return;

        _JobExplain = CoreManagers.Data.LoadCSV("Data/Character/JobExplain");

        _isLoaded = true;
    }

    public object GetJobExplain(Define.Job job, string name)
    {
        return _JobExplain[(int)job][name];
    }
}
