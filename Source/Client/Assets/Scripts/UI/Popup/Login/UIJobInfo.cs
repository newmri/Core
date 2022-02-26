using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using System;

public class UIJobInfo : UIPopup
{
    [SerializeField]
    Define.Job _job;

    enum Images
    {
        Icon
    }

    enum TextMeshProUGUIs
    {
        JobText,
        JobExplain
    }

    enum GameObjects
    {
        HP,
        MP,
        Damage,
        MagicDamage,
        Defence,
        MagicDefence
    }

    public override void Init()
    {
        base.Init();
        Bind<Image>(typeof(Images));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
        Bind<GameObject>(typeof(GameObjects));

        gameObject.SetActive(false);
    }

    public void OnJobSelect(Define.Job job)
    {
        _job = job;

        SetExplain();

        gameObject.SetActive(true);
    }

    private void SetExplain()
    {
        Sprite jobIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{_job.ToString()}Icon");
        GetImage((int)Images.Icon).sprite = jobIcon;

        this.GetTextMesh((int)TextMeshProUGUIs.JobText).text =
            Managers.LoginData.GetJobExplain(_job, "Name").ToString();
        this.GetTextMesh((int)TextMeshProUGUIs.JobExplain).text =
            Managers.LoginData.GetJobExplain(_job, "Explain").ToString().Replace("\\n", "\n");

        foreach (GameObjects stat in Enum.GetValues(typeof(GameObjects)))
        {
            GetObject((int)stat).GetComponentInChildren<Slider>().value = 
                (float)Managers.LoginData.GetJobExplain(_job, stat.ToString());
        }
    }
}
