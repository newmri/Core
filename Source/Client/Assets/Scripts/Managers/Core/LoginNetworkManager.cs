using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using UnityCoreLibrary;
using FlatBuffers;
using Login;
using System.Threading;

public class LoginNetworkManager
{
    public long UID { get; set; }
    public int Token { get; set; }
    public string ID { get; set; }
    public string Password { get; set; }

    LoginServerSession _session = new LoginServerSession();

    public void Send(FlatBufferBuilder packet)
    {
        _session.Send(packet);
    }

    public void Conntect(ServerInfo info)
    {
        if (_session.IsConnected)
        {
            Managers.LoginNetwork.Send(Packet.CS_LOGOUT_NOTI);
            Thread.Sleep(100);
            _session.Disconnect();
            _session = new LoginServerSession();
            Thread.Sleep(100);
        }

        IPAddress ipAddr = IPAddress.Parse(info.ServerIP);
        IPEndPoint endPoint = new IPEndPoint(ipAddr, info.ServerPort);

        Connector connector = new Connector();

        connector.Connect(endPoint,
            () => { return _session; },
            1);
    }

    public void Update()
    {
        List<LoginPacketMessage> list = LoginPacketQueue.Instance.PopAll();
        foreach (LoginPacketMessage packet in list)
        {
            Action<PacketSession, Root> handler = LoginPacketManager.Instance.GetPacketHandler(packet.ID);
            if (handler != null)
                handler.Invoke(_session, packet.Message);
        }
    }

    [Obsolete]
    public void WorldListServerLogin()
    {
        LoginAccountPacketReq packet = new LoginAccountPacketReq()
        {
            ID = ID,
            Password = Password
        };

        Managers.Web.SendPostRequest<LoginAccountPacketRes>("login", packet, (res) =>
        {
            if (res.IsSuccess)
            {
                Managers.LoginNetwork.UID = res.UID;
                Managers.LoginNetwork.Token = res.Token;
                Managers.UI.ShowPopupUI<UIWorldListPopup>().SetWorldList(res.WorldList);
            }
            else
            {
                UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();
                messagePopup.SetText("로그인 실패", "아이디 또는 비밀번호가 틀렸습니다\n 다시 입력해주세요");
            }
        });
    }

    public void SendLogin()
    {
        FlatBufferBuilder builder = new FlatBufferBuilder(1);
        var message = CS_LOGIN_REQ.CreateCS_LOGIN_REQ(builder, UID, Token);
        var data = Root.CreateRoot(builder, Packet.CS_LOGIN_REQ, message.Value);
        builder.Finish(data.Value);
        Send(builder);
    }

    public void Send(Packet message)
    {
        FlatBufferBuilder builder = new FlatBufferBuilder(1);
        Root.StartRoot(builder);
        Root.AddPacketType(builder, message);
        var data = Root.EndRoot(builder);
        builder.Finish(data.Value);
        Send(builder);
    }
}
