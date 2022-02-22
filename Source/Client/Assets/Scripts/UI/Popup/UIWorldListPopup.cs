using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class UIWorldListPopup : UIPopup
{
    public List<UIWorldListPopupInfo> WorldList { get; } = new List<UIWorldListPopupInfo>();

    enum Buttons
    {
        LoginButton,
        CloseButton
    }

    public override void Init()
    {
        base.Init();
        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.LoginButton).gameObject.BindEvent(OnClickLoginButton);
        GetButton((int)Buttons.CloseButton).gameObject.BindEvent(OnClickCloseButton);
    }

    public void SetWorldList(List<WorldListInfo> worldListInfos)
    {
        WorldList.Clear();

        GameObject grid = GetComponentInChildren<GridLayoutGroup>().gameObject;
        foreach (Transform child in grid.transform)
            Destroy(child.gameObject);

        for (int i = 0; i < worldListInfos.Count; ++i)
        {
            GameObject go = CoreManagers.Resource.Instantiate("UI/Popup/UIWorldListPopupInfo", grid.transform);
            UIWorldListPopupInfo info = go.GetOrAddComponent<UIWorldListPopupInfo>();
            info.Info = worldListInfos[i];
            WorldList.Add(info);
        }
    }

    [System.Obsolete]
    public void OnClickLoginButton(PointerEventData evt)
    {
        if (WorldList.Count == 0)
            return;

        int WorldID = GetSelectedWorldID();

        LoginServerInfoPacketReq packet = new LoginServerInfoPacketReq()
        {
            WorldID = WorldID
        };

        Managers.Web.SendPostRequest<LoginServerInfoPacketRes>("serverselect", packet, (res) =>
        {
            Managers.LoginNetwork.Conntect(res);
        });
    }

    int GetSelectedWorldID()
    {
        foreach (var worldInfo in WorldList)
        {
            if (worldInfo.IsSelected)
            {
                return worldInfo.Info.ID;
            }
        }

        return 0;
    }

    public void OnClickCloseButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI(true);
    }
}
