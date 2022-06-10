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
    protected ServerSession _session;
    public ServerInfo ServerInfo;

    public virtual void Init()
    {

    }

    public virtual void Conntect()
    {
        if (_session.IsConnected)
        {
            Disconnect();
        }

        IPAddress ipAddr = IPAddress.Parse(ServerInfo.ServerIP);
        IPEndPoint endPoint = new IPEndPoint(ipAddr, ServerInfo.ServerPort);

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
