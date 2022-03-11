using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using UnityCoreLibrary;
using FlatBuffers;
using GamePacket;
using System.Threading;

public class GameNetworkManager : BaseNetworkManager
{
    public long CharacterUID { get; set; }

    public override void Init()
    {
        base.Init();
        _session = new GameServerSession();
    }

    public override void Conntect(ServerInfo info)
    {
        base.Conntect(info);
    }

    public override void Disconnect(bool newSession = true)
    {
        Send(Packet.CS_LOGOUT_NOTI);
        Thread.Sleep(100);
        _session.Disconnect();

        if (newSession)
            _session = new LoginServerSession();

        Init();
        Thread.Sleep(100);
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
        var message = CS_LOGIN_REQ.CreateCS_LOGIN_REQ(builder, Managers.Account.UID, CharacterUID,
            Managers.Account.Token);
        Send(builder, Packet.CS_LOGIN_REQ, message.Value);
    }

    public void Update()
    {
        if (_session.IsConnected)
        {
            List<GamePacketMessage> list = GamePacketQueue.Instance.PopAll();
            foreach (GamePacketMessage packet in list)
            {
                Action<PacketSession, Root> handler = GamePacketManager.Instance.GetPacketHandler(packet.ID);
                if (handler != null)
                    handler.Invoke(_session, packet.Message);
            }
        }
    }
}
