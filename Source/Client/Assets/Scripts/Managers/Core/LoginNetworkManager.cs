﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using UnityCoreLibrary;
using FlatBuffers;
using Login;

public class LoginNetworkManager
{
    public long UID { get; set; }
    public int Token { get; set; }

    public bool IsLoginSuccess { get; set; }

    LoginServerSession _session = new LoginServerSession();

    public void Send(FlatBufferBuilder packet)
    {
        _session.Send(packet);
    }

    public void Conntect(ServerInfo info)
    {
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

    public void CheckLogin()
    {
        if (!IsLoginSuccess)
        {
            FlatBufferBuilder builder = new FlatBufferBuilder(1);
            var message = CS_LOGIN_REQ.CreateCS_LOGIN_REQ(builder, UID, Token);
            var data = Root.CreateRoot(builder, Packet.CS_LOGIN_REQ, message.Value);
            builder.Finish(data.Value);
            Send(builder);
        }
    }
}