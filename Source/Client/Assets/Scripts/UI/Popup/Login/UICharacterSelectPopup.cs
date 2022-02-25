using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class UICharacterSelectPopup : UIPopup
{
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

    public void SetSlot(byte emptySlotCount, byte lockSlotCount)
    {
        _gird = GetHorizontalLayoutGroup();

        for (byte i = 0; i < emptySlotCount; ++i)
            AddEmptySlot();

        for (byte i = 0; i < lockSlotCount; ++i)
            AddLockSlot();
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

    [System.Obsolete]
    public void OnClickBackButton(PointerEventData evt)
    {
        Managers.UI.HideCurrentSceneUI();
        Managers.UI.CloseAllPopupUI();
        Managers.LoginNetwork.WorldListServerLogin();
    }
}
