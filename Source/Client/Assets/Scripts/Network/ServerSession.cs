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
    }

    public override void OnDisconnected(EndPoint endPoint)
    {
        Debug.Log($"OnDisconnected : {endPoint}");
    }

    public override void OnRecvPacket(ArraySegment<byte> buffer)
    {
       
    }

    public override void OnSend(int numOfBytes)
    {

    }
}