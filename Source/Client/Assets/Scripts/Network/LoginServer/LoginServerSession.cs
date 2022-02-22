using UnityCoreLibrary;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using FlatBuffers;
using Login;

public class LoginServerSession : ServerSession
{
    public override void OnConnected(EndPoint endPoint)
    {
        base.OnConnected(endPoint);

        LoginPacketManager.Instance.CustomHandler = (s, m, i) =>
        {
            LoginPacketQueue.Instance.Push(m, i);
        };

        FlatBufferBuilder builder = new FlatBufferBuilder(1);
        var message = CS_LOGIN_REQ.CreateCS_LOGIN_REQ(builder, Managers.LoginNetwork.UID, Managers.LoginNetwork.Token);
        var data = Root.CreateRoot(builder, Packet.CS_LOGIN_REQ, message.Value);
        builder.Finish(data.Value);
        Managers.LoginNetwork.Send(builder);
    }

    public override void OnDisconnected(EndPoint endPoint)
    {
        base.OnDisconnected(endPoint);
    }

    public override void OnRecvPacket(ArraySegment<byte> buffer)
    {
        base.OnRecvPacket(buffer);
        LoginPacketManager.Instance.OnRecvPacket(this, buffer);
    }

    public override void OnSend(int numOfBytes)
    {
        base.OnSend(numOfBytes);
    }
}