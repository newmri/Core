using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using System;

public class UIJobButton : UIPopup
{
    [SerializeField]
    Color _selectColor = new Color(251, 244, 190);

    [SerializeField]
    Define.Job _job;

    enum Buttons
    {
        JobButton
    }

    enum Images
    {
        SelectFrame,
        Icon
    }

    enum TextMeshProUGUIs
    {
        Text
    }

    public override void Init()
    {
        base.Init();

        Bind<Button>(typeof(Buttons));
        Bind<Image>(typeof(Images));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));

        GetButton((int)Buttons.JobButton).gameObject.BindEvent(OnClickJobButton);

        Sprite jobIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{_job.ToString()}Icon");
        GetImage((int)Images.Icon).sprite = jobIcon;

        this.GetTextMesh((int)TextMeshProUGUIs.Text).text = 
            Managers.LoginData.GetJobExplain(_job, "Name").ToString();

        Deselected();
    }

    public void SetJob(Define.Job job)
    {
        _job = job;
    }

    public void OnJobSelect(Define.Job job)
    {
        if (_job == job)
            Selected();
        else
            Deselected();
    }

    private void Selected()
    {
        GetImage((int)Images.SelectFrame).gameObject.SetActive(true);
        GetImage((int)Images.Icon).color = _selectColor;
        this.GetTextMesh((int)TextMeshProUGUIs.Text).color = _selectColor;
    }

    private void Deselected()
    {
        GetImage((int)Images.SelectFrame).gameObject.SetActive(false);
        GetImage((int)Images.Icon).color = Color.white;
        this.GetTextMesh((int)TextMeshProUGUIs.Text).color = Color.white;
    }

    public void OnClickJobButton(PointerEventData evt)
    {
        transform.GetComponentInParent<UICharacterCreatePopup>().OnJobSelect(_job);
    }
}
