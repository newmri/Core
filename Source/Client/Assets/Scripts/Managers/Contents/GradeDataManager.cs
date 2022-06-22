using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;
using Define;
using System;

public struct GradeInfo
{
    public string name;
    public Sprite frame;
    public Color color;
}

public class GradeDataManager
{
    bool _isLoaded = false;
    public GradeInfo[] GradeInfoList;

    public void Load()
    {
        if (_isLoaded)
            return;

        _isLoaded = true;

        GradeInfoList = new GradeInfo[(int)GradeType.END]
        {
            new GradeInfo {name = "ÀÏ¹Ý", color = Color.white },
            new GradeInfo {name = "Áß±Þ", color = new Color(0.2403033f, 0.6226415f, 0.09104663f, 1.0f)},
            new GradeInfo {name = "°í±Þ", color = new Color(0.6235294f, 0.5189935f, 0.09019609f, 1.0f)},
            new GradeInfo {name = "Èñ±Í", color = new Color(0.2587219f, 0.5067571f, 0.9622642f, 1.0f)},
            new GradeInfo {name = "¿µ¿õ", color = new Color(0.519282f, 0.2588235f, 0.9607843f, 1.0f)},
            new GradeInfo {name = "Àü¼³", color = new Color(1.0f, 0.6588226f, 0.2122642f, 1.0f)},
            new GradeInfo {name = "½ÅÈ­", color = new Color(0.9339623f, 0.09973177f, 0.0572713f, 1.0f)}
        };

        for(GradeType gradeType = 0; gradeType < GradeType.END; ++gradeType)
        {
            GradeInfoList[(int)gradeType].frame = CoreManagers.Resource.Load<Sprite>($"UI/Frame/Grade/{Util.EnumToPascal(gradeType.ToString())}Frame");
        }
    }
}
