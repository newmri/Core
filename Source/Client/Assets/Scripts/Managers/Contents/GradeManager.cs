using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;
using Define;

public struct GradeInfo
{
    public string name;
    public Sprite frame;
}

public class GradeManager
{
    bool _isLoaded = false;
    private GradeInfo[] _gradeInfoList;

    public void Load()
    {
        if (_isLoaded)
            return;

        _isLoaded = true;

        _gradeInfoList = new GradeInfo[(int)GradeType.END]
        {
            new GradeInfo {name = "ÀÏ¹Ý"},
            new GradeInfo {name = "Áß±Þ"},
            new GradeInfo {name = "°í±Þ"},
            new GradeInfo {name = "Èñ±Í"},
            new GradeInfo {name = "¿µ¿õ"},
            new GradeInfo {name = "Àü¼³"},
            new GradeInfo {name = "½ÅÈ­"}
        };

        for(GradeType gradeType = 0; gradeType < GradeType.END; ++gradeType)
        {
            _gradeInfoList[(int)gradeType].frame = CoreManagers.Resource.Load<Sprite>($"UI/Frame/Grade/{Util.EnumToPascal(gradeType.ToString())}Frame");
        }
    }
}
