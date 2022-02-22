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

        Managers.LoginNetwork.CheckLogin();
    }

    public override void OnDisconnected(EndPoint endPoint)
    {
        base.OnDisconnected(endPoint);
        Managers.LoginNetwork.IsLoginSuccess = false;
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