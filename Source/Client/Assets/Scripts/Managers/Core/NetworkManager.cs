using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using UnityCoreLibrary;
using FlatBuffers;

public class ServerInfo
{
    public string ServerIP;
    public int ServerPort;
}

public class NetworkManager
{
    public long UID { get; set; }
    public long Token { get; set; }

    ServerSession _session = new ServerSession();

    public void Send(FlatBufferBuilder packet)
    {
        _session.Send(packet);
    }

    public void ConntectToGame(ServerInfo info)
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
        List<PacketMessage> list = PacketQueue.Instance.PopAll();
        foreach (PacketMessage packet in list)
        {
            Action<PacketSession, IFlatbufferObject> handler = PacketManager.Instance.GetPacketHandler(packet.Id);
            if (handler != null)
                handler.Invoke(_session, packet.Message);
        }
    }

}
