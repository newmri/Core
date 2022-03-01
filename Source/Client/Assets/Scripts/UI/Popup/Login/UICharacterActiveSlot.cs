using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using System;
using Login;
using Spine.Unity;

public class UICharacterActiveSlot : UIPopup
{
    [SerializeField]
    CHARACTER_INFO _info;

    bool _IsSelected = false;

    enum Buttons
    {
        UICharacterActiveSlot
    }

    enum Images
    {
        Icon
    }

    enum TextMeshProUGUIs
    {
        Info
    }

    enum GameObjects
    {
        CharacterBgDefault,
        CharacterBgActive,
        Character,
        CharacterGFX
    }

    public override void Init()
    {
        base.Init();

        Bind<Button>(typeof(Buttons));
        Bind<Image>(typeof(Images));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
        Bind<GameObject>(typeof(GameObjects));

        GetObject((int)GameObjects.CharacterBgActive).SetActive(false);
        GetButton((int)Buttons.UICharacterActiveSlot).gameObject.BindEvent(OnClickSlotButton);

        UpdateSlot();
    }

    private void UpdateSlot()
    {
        Sprite jobIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{_info.Job.ToString()}Icon");
        GetImage((int)Images.Icon).sprite = jobIcon;

        string explain = "Lv{0}.{1}\n{2}";
        explain = string.Format(explain, _info.Level.ToString(), _info.Name,
                                Managers.LoginData.GetJobExplain(_info.Job, "Name").ToString());
        this.GetTextMesh((int)TextMeshProUGUIs.Info).text = explain;

        GearEquipper gear = GetObject((int)GameObjects.Character).GetComponent<GearEquipper>();
        gear.Job = _info.Job;
        gear.SetGear(_info.GetGearArray());

        MeshRenderer mesh = GetObject((int)GameObjects.CharacterGFX).GetComponent<MeshRenderer>();
        mesh.sortingOrder = GetComponent<Canvas>().sortingOrder + 1;
    }

    public void OnClickSlotButton(PointerEventData evt)
    {
        transform.GetComponentInParent<UICharacterSelectPopup>().OnActiveSlotSelected();
        OnSelected();
    }

    public void OnSelected()
    {
        _IsSelected = true;
        GetObject((int)GameObjects.CharacterBgDefault).SetActive(false);
        GetObject((int)GameObjects.CharacterBgActive).SetActive(true);
    }

    public void OnDeselected()
    {
        _IsSelected = false;
        GetObject((int)GameObjects.CharacterBgDefault).SetActive(true);
        GetObject((int)GameObjects.CharacterBgActive).SetActive(false);
    }

    public void SetInfo(CHARACTER_INFO info)
    {
        transform.SetAsFirstSibling();
        _info = info;
    }

}
