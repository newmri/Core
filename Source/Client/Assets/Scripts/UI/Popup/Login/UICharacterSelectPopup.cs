using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using LoginPacket;

public class UICharacterSelectPopup : UIPopup
{
    public List<UICharacterActiveSlot> _activeList { get; } = new List<UICharacterActiveSlot>();
    public List<UICharacterEmptySlot> _emptyList { get; } = new List<UICharacterEmptySlot>();
    public List<UICharacterLockSlot> _lockList { get; } = new List<UICharacterLockSlot>();

    GameObject _gird;

    enum Buttons
    {
        BackButton
    }

    public override void Init()
    {
        base.Init();

        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.BackButton).gameObject.BindEvent(OnClickBackButton);
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

    public void SetActiveSlot(CHARACTER_INFO info)
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

    [System.Obsolete]
    public void OnClickBackButton(PointerEventData evt)
    {
        Managers.UI.HideCurrentSceneUI();
        Managers.UI.CloseAllPopupUI();
        Managers.LoginNetwork.WorldListServerLogin();
    }
}
