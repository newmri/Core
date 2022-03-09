using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using UnityCoreLibrary;
using FlatBuffers;
using System.Threading;

public class BaseNetworkManager
{
    public int WorldID { get; set; }
    public long UID { get; set; }
    public int Token { get; set; }

    protected ServerSession _session;

    public virtual void Init()
    {

    }

    public virtual void Conntect(ServerInfo info)
    {
        if (_session.IsConnected)
        {
            Disconnect();
        }

        IPAddress ipAddr = IPAddress.Parse(info.ServerIP);
        IPEndPoint endPoint = new IPEndPoint(ipAddr, info.ServerPort);

        Connector connector = new Connector();

        connector.Connect(endPoint,
            () => { return _session; },
            1);
    }

    public virtual void Disconnect(bool newSession = true)
    {

    }

    public void Send(FlatBufferBuilder packet)
    {
       _session.Send(packet);
    }

    public virtual void SendLogin()
    {
    }
}
