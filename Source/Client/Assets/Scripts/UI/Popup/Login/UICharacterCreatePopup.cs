using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using System;

public class UICharacterCreatePopup : UIPopup
{
    GameObject _gird;
    List<UIJobButton> _jobButton = new List<UIJobButton>();
    Define.Job _selectedJob;

    enum GameObjects
    {
        JobInfo
    }

    enum Buttons
    {
        CreateButton,
        BackButton
    }

    public override void Init()
    {
        base.Init();
        Bind<GameObject>(typeof(GameObjects));
        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.CreateButton).gameObject.BindEvent(OnClickCreateButton);
        GetButton((int)Buttons.BackButton).gameObject.BindEvent(OnClickBackButton);
    }

    public void SetJobButtons()
    {
        _gird = GetEmptyGridLayoutGroup();

        foreach (Define.Job job in Enum.GetValues(typeof(Define.Job)))
        {
            GameObject go = CoreManagers.Resource.Instantiate("UI/Popup/UIJobButton", _gird.transform);
            UIJobButton jobButton = go.GetOrAddComponent<UIJobButton>();
            jobButton.SetJob(job);
            _jobButton.Add(jobButton);
        }
    }

    public void OnJobSelect(Define.Job job)
    {
        _selectedJob = job;

        foreach (var jobButton in _jobButton)
            jobButton.OnJobSelect(job);

        GetObject((int)GameObjects.JobInfo).GetComponent<UIJobInfo>().OnJobSelect(job);
    }

    public void OnClickCreateButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }

    public void OnClickBackButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }
}
