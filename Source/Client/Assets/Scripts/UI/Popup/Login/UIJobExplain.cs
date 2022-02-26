using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using System;

public class UIJobExplain : UIPopup
{
    [SerializeField]
    Define.Job _job;

    enum Images
    {
        Icon
    }

    enum TextMeshProUGUIs
    {
        Text
    }

    public override void Init()
    {
        base.Init();
        Bind<Image>(typeof(Images));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));

        gameObject.SetActive(false);
    }

    public void OnJobSelect(Define.Job job)
    {
        _job = job;

        Sprite jobIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{_job.ToString()}Icon");
        GetImage((int)Images.Icon).sprite = jobIcon;
        gameObject.SetActive(true);
    }
}
