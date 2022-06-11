using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class UILoadingScene : UIScene
{
    enum Sliders
    {
        LoadingSlider
    }

    enum TextMeshProUGUIs
    {
        LoadingText
    }

    public override void Init()
    {
        base.Init();

        Bind<Slider>(typeof(Sliders));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
    }

    private void Update()
    {
        float amount = CoreManagers.Scene.LoadingAmount * 100.0f;
        GetSlider((int)Sliders.LoadingSlider).value = amount;
        this.GetTextMesh((int)TextMeshProUGUIs.LoadingText).text = "Loading.... " + Mathf.RoundToInt(amount) + "%";
    }
}
