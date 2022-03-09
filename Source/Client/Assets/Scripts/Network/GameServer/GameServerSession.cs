using UnityCoreLibrary;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using FlatBuffers;
using GamePacket;

public class GameServerSession : ServerSession
{
    public override void OnConnected(EndPoint endPoint)
    {
        base.OnConnected(endPoint);

        GamePacketManager.Instance.CustomHandler = (s, m, i) =>
        {
            GamePacketQueue.Instance.Push(m, i);
        };

        Managers.GameNetwork.SendLogin();
    }

    public override void OnDisconnected(EndPoint endPoint)
    {
        base.OnDisconnected(endPoint);
    }

    public override void OnRecvPacket(ArraySegment<byte> buffer)
    {
        base.OnRecvPacket(buffer);
        GamePacketManager.Instance.OnRecvPacket(this, buffer);
    }

    public override void OnSend(int numOfBytes)
    {
        base.OnSend(numOfBytes);
    }
}