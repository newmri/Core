using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using UnityCoreLibrary;
using FlatBuffers;
using GamePacket;
using System.Threading;

public class GameNetworkManager
{
    public long UID { get; set; }
    public int Token { get; set; }

    GameServerSession _session = new GameServerSession();

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

    public void Send(FlatBufferBuilder packet)
    {
        _session.Send(packet);
    }

    public void Conntect(ServerInfo info)
    {
        if (_session.IsConnected)
        {
            Managers.GameNetwork.Send(Packet.CS_LOGOUT_NOTI);
            Thread.Sleep(100);
            _session.Disconnect();
            _session = new GameServerSession();
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
        List<GamePacketMessage> list = GamePacketQueue.Instance.PopAll();
        foreach (GamePacketMessage packet in list)
        {
            Action<PacketSession, Root> handler = GamePacketManager.Instance.GetPacketHandler(packet.ID);
            if (handler != null)
                handler.Invoke(_session, packet.Message);
        }
    }

    public void SendLogin()
    {
        FlatBufferBuilder builder = new FlatBufferBuilder(1);
        var message = CS_LOGIN_REQ.CreateCS_LOGIN_REQ(builder, UID, Token);
        Send(builder, Packet.CS_LOGIN_REQ, message.Value);
    }
}
