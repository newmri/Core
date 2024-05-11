using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using UnityCoreLibrary;
using FlatBuffers;
using LoginPacket;
using System.Threading;
using Define;

public class LoginNetworkManager : BaseNetworkManager
{
    public override void Init()
    {
        base.Init();
        _session = new LoginServerSession();
    }

    public override void Conntect()
    {
        base.Conntect();
    }

    public override void Disconnect(bool newSession = true)
    {
        if (_session.IsConnected)
        {
            Send(Packet.CS_LOGOUT_NOTI);
            Thread.Sleep(100);
            _session.Disconnect();

            if (newSession)
                _session = new LoginServerSession();

            Thread.Sleep(100);
        }
    }

    public void Send(FlatBufferBuilder packet, Packet packetType, int packetOffset)
    {
        var data = Root.CreateRoot(packet, packetType, packetOffset);
        packet.Finish(data.Value);
        Send(packet);
    }

    public void Send(Packet packetType)
    {
        FlatBufferBuilder builder = new FlatBufferBuilder(1);
        Root.StartRoot(builder);
        Root.AddPacketType(builder, packetType);
        var data = Root.EndRoot(builder);
        builder.Finish(data.Value);
        Send(builder);
    }

    public override void SendLogin()
    {
        FlatBufferBuilder builder = new FlatBufferBuilder(1);
        var message = CS_LOGIN_REQ.CreateCS_LOGIN_REQ(builder, Managers.Account.UID, Managers.Account.Token);
        Send(builder, Packet.CS_LOGIN_REQ, message.Value);
    }

    public void Update()
    {
        if (_session.IsConnected)
        {
            List<LoginPacketMessage> list = LoginPacketQueue.Instance.PopAll();
            foreach (LoginPacketMessage packet in list)
            {
                Action<PacketSession, Root> handler = LoginPacketManager.Instance.GetPacketHandler(packet.ID);
                if (handler != null)
                    handler.Invoke(_session, packet.Message);
            }
        }
    }

    [Obsolete]
    public void WorldListServerLogin()
    {
        LoginAccountPacketReq packet = new LoginAccountPacketReq()
        {
            ID = Managers.Account.ID,
            Password = Managers.Account.Password
        };

        Managers.Web.SendPostRequest<LoginAccountPacketRes>("login", packet, (res) =>
        {
            UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();

            switch (res.Code)
            {
                case WebLoginResultCode.SUCCESS:
                    Managers.Account.UID = res.UID;
                    Managers.Account.Token = res.Token;

                    Managers.UI.CloseAllPopupUI();
                    Managers.UI.ShowPopupUI<UIWorldListPopup>().SetWorldList(res.WorldList);
                    break;
                case WebLoginResultCode.LOGINED:
                    messagePopup.SetText("로그인 실패", "이미 접속중입니다");
                    break;
                case WebLoginResultCode.WRONG:
                    messagePopup.SetText("로그인 실패", "아이디 또는 비밀번호가 틀렸습니다\n 다시 입력해주세요");
                    break;
            }
        });
    }
}
