using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;

public class LoginDataManager : MonoBehaviour
{
    List<Dictionary<string, object>> _JobExplain;

    public void Load()
    {
        _JobExplain = CoreManagers.Data.LoadCSV("Data/Character/JobExplain");
    }

    public object GetJobExplain(Define.Job job, string name)
    {
        return _JobExplain[(int)job][name];
    }
}
