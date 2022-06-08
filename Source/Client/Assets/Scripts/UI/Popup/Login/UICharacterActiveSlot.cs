using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using System;
using LoginPacket;
using Spine.Unity;
using FlatBuffers;

public class UICharacterActiveSlot : UIPopup
{
    [SerializeField]
    LoginPacket.CharacterInfo _info;

    bool _IsSelected = false;

    enum Buttons
    {
        UICharacterActiveSlot,
        DeleteButton
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
        GetButton((int)Buttons.DeleteButton).gameObject.BindEvent(OnClickDeleteButton);


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
        gear.SetGear(_info.Gear.Value.UnPack());

        MeshRenderer mesh = GetObject((int)GameObjects.CharacterGFX).GetComponent<MeshRenderer>();
        mesh.sortingOrder = GetComponent<Canvas>().sortingOrder + 1;
    }

    public void OnClickSlotButton(PointerEventData evt)
    {
        transform.GetComponentInParent<UICharacterSelectPopup>().OnActiveSlotSelected();
        OnSelected();
    }

    public void OnClickDeleteButton(PointerEventData evt)
    {
        UINameMessagePopup nameMessagePopup = Managers.UI.ShowPopupUI<UINameMessagePopup>();
        nameMessagePopup.SetText("캐릭터 삭제", "캐릭터명", "캐릭터를 삭제하면 되돌릴 수 없습니다.\n 신중히 선택하세요.",
         (evt) =>
         {
             if (_info.Name != nameMessagePopup.Name)
             {
                 UIErrorMessagePopup errorMessagePopup = Managers.UI.ShowPopupUI<UIErrorMessagePopup>();
                 errorMessagePopup.SetText("캐릭터명이 잘못 되었습니다.");
             }
             else
             {
                 FlatBufferBuilder builder = new FlatBufferBuilder(1);
                 var message = CS_DELETE_CHARACTER_REQ.CreateCS_DELETE_CHARACTER_REQ(builder, _info.Uid);
                 Managers.LoginNetwork.Send(builder, Packet.CS_DELETE_CHARACTER_REQ, message.Value);
             }
         });
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

    public void SetInfo(LoginPacket.CharacterInfo info)
    {
        transform.SetAsFirstSibling();
        _info = info;
    }

    public bool IsSelected()
    {
        return _IsSelected;
    }

    public long GetUID()
    {
        return _info.Uid;
    }
}
