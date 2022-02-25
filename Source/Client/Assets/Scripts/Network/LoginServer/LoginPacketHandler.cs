using UnityCoreLibrary;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using FlatBuffers;
using Login;

class LoginPacketHandler
{
    public static void SC_LOGIN_RES(PacketSession session, Root packet)
    {
        SC_LOGIN_RES loginRes = packet.PacketAsSC_LOGIN_RES();
        if (ErrorCode.SUCCESS != loginRes.Result)
        {
            Managers.UI.CloseAllPopupUI();

            UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();

            switch (loginRes.Result)
            {
                case ErrorCode.UNKNOWN:
                    messagePopup.SetText("서버 접속 실패", "다시 로그인을 해주세요");
                    break;
                case ErrorCode.ALREADY_LOGINED:
                    messagePopup.SetText("서버 접속 실패", "이미 접속 중 입니다");
                    break;
            }

            session.Disconnect();
            return;
        }

        UICharacterSelectPopup popUp = Managers.UI.ShowPopupUI<UICharacterSelectPopup>();
        popUp.SetSlot(loginRes.EmptySlotCount, (byte)(loginRes.MaxSlotCount - loginRes.EmptySlotCount));
    }

    public static void SC_PING_REQ(PacketSession session, Root packet)
    {
        Debug.Log("PingCheck");
        Managers.LoginNetwork.Send(Packet.CS_PING_RES);
    }
}


