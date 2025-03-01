using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using LoginPacket;
using System;
using Define;
using System.Linq;

public class UICharacterSelectPopup : UIPopup
{
    public List<UICharacterActiveSlot> _activeList { get; } = new List<UICharacterActiveSlot>();
    public List<UICharacterEmptySlot> _emptyList { get; } = new List<UICharacterEmptySlot>();
    public List<UICharacterLockSlot> _lockList { get; } = new List<UICharacterLockSlot>();

    GameObject _gird;

    enum GameObjects
    {
        CharacterSlotScroll
    }

    enum Buttons
    {
        BackButton,
        StartButton
    }

    public override void Init()
    {
        base.Init();

        Bind<GameObject>(typeof(GameObjects));
        Bind<Button>(typeof(Buttons));

        GetObject((int)GameObjects.CharacterSlotScroll).GetComponent<ScrollRect>().horizontalNormalizedPosition = 0;

        GetButton((int)Buttons.BackButton).gameObject.BindEvent(OnClickBackButton);
        GetButton((int)Buttons.StartButton).gameObject.BindEvent(OnClickStartButton);
    }

    public void Clear()
    {
        _gird = GetHorizontalLayoutGroup();
    }

    public void SetSlot(byte emptySlotCount, byte lockSlotCount)
    {
        for (byte i = 0; i < emptySlotCount; ++i)
            AddEmptySlot();

        for (byte i = 0; i < lockSlotCount; ++i)
            AddLockSlot();
    }

    public void SetActiveSlot(LoginPacket.CharacterInfo info)
    {
        GameObject go = CoreManagers.Resource.Instantiate("UI/Popup/UICharacterActiveSlot", _gird.transform);
        
        UICharacterActiveSlot slot = go.GetOrAddComponent<UICharacterActiveSlot>();
        slot.SetInfo(info);

        _activeList.Add(slot);
    }

    public void AddEmptySlot()
    {
        GameObject go = CoreManagers.Resource.Instantiate("UI/Popup/UICharacterEmptySlot", _gird.transform);
        _emptyList.Add(go.GetOrAddComponent<UICharacterEmptySlot>());
    }

    public void AddLockSlot()
    {
        GameObject go = CoreManagers.Resource.Instantiate("UI/Popup/UICharacterLockSlot", _gird.transform);
        _lockList.Add(go.GetOrAddComponent<UICharacterLockSlot>());
    }

    public void DeleteActiveSlot(long uid)
    {
        UICharacterActiveSlot slot = _activeList.Find(activeSlot => uid == activeSlot.GetUID());
        if (null != slot)
        {
            _activeList.Remove(slot);
            CoreManagers.Resource.Destroy(slot.gameObject);

            AddEmptySlot();

            UICharacterEmptySlot addedSlot = _emptyList[_emptyList.Count - 1];
            addedSlot.transform.SetSiblingIndex(_activeList.Count() + _emptyList.Count());
        }
    }

    public void RemoveLastEmptySlot()
    {
        if (0 == _emptyList.Count)
            return;

        UICharacterEmptySlot slot = _emptyList[_emptyList.Count - 1];
        _emptyList.Remove(slot);
        CoreManagers.Resource.Destroy(slot.gameObject);
    }

    public void OnActiveSlotSelected()
    {
        foreach (UICharacterActiveSlot slot in _activeList)
            slot.OnDeselected();
    }

    public void OnClickBackButton(PointerEventData evt)
    {
        Managers.UI.HideCurrentSceneUI();
        Managers.UI.CloseAllPopupUI();
        Managers.LoginNetwork.WorldListServerLogin();
    }

    [Obsolete]
    public void OnClickStartButton(PointerEventData evt)
    {
        if (!SetGameInfo())
        {
            UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();
            messagePopup.SetText("서버 접속 실패", "캐릭터를 먼저 선택 해주세요");
            return;
        }

        LoginServerInfoPacketReq packet = new LoginServerInfoPacketReq()
        {
            WorldID = Managers.Account.WorldID,
            ServerType = ServerType.Game
        };

        Managers.Web.SendPostRequest<LoginServerInfoPacketRes>("serverselect", packet, (res) =>
        {
            Managers.LoginNetwork.Disconnect();
            Managers.GameNetwork.ServerInfo = res;
            Managers.GameNetwork.Conntect();
        });
    }

    bool SetGameInfo()
    {
        foreach (UICharacterActiveSlot slot in _activeList)
        {
            if (slot.IsSelected())
            {
                Managers.GameNetwork.CharacterUID = slot.GetUID();
                return true;
            }
        }

        return false;
    }
}
