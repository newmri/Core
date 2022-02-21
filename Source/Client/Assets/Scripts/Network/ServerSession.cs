using UnityCoreLibrary;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using FlatBuffers;

public class ServerSession : PacketSession
{
    public void Send(FlatBufferBuilder packet)
    {
        int size = packet.Offset;
        byte[] sendBuffer = new byte[size + 4];
        Array.Copy(BitConverter.GetBytes(size), 0, sendBuffer, 0, sizeof(int));
        Array.Copy(packet.DataBuffer.ToArray(packet.DataBuffer.Position, size), 0, sendBuffer, sizeof(int), size);
        Send(new ArraySegment<byte>(sendBuffer));
    }

    public override void OnConnected(EndPoint endPoint)
    {
        Debug.Log($"OnConnected : {endPoint}");

        PacketManager.Instance.CustomHandler = (s, m, i) =>
        {
            PacketQueue.Instance.Push(i, m);
        };

        FlatBufferBuilder builder = new FlatBufferBuilder(1);
        var message = Login.CS_LOGIN_REQ.CreateCS_LOGIN_REQ(builder, Managers.Network.UID, Managers.Network.Token);
        var data = Login.Root.CreateRoot(builder, Login.Packet.CS_LOGIN_REQ, message.Value);
        builder.Finish(data.Value);
        Managers.Network.Send(builder);
    }

    public override void OnDisconnected(EndPoint endPoint)
    {
        Debug.Log($"OnDisconnected : {endPoint}");
    }

    public override void OnRecvPacket(ArraySegment<byte> buffer)
    {
        PacketManager.Instance.OnRecvPacket(this, buffer);
    }

    public override void OnSend(int numOfBytes)
    {
       
    }
}