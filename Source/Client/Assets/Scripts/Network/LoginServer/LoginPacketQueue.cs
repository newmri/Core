using System;
using FlatBuffers;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Login;

public class LoginPacketMessage
{
    public Packet ID { get; set; }
    public Root Message { get; set; }
}

public class LoginPacketQueue
{
    public static LoginPacketQueue Instance { get; } = new LoginPacketQueue();

    Queue<LoginPacketMessage> _packetQueue = new Queue<LoginPacketMessage>();
    object _lock = new object();

    public void Push(Packet id, Root packet)
    {
        lock (_lock)
        {
            _packetQueue.Enqueue(new LoginPacketMessage() { ID = id, Message = packet });
        }
    }

    public LoginPacketMessage Pop()
    {
        lock (_lock)
        {
            if (_packetQueue.Count == 0)
                return null;

            return _packetQueue.Dequeue();
        }
    }

    public List<LoginPacketMessage> PopAll()
    {
        List<LoginPacketMessage> list = new List<LoginPacketMessage>();

        lock (_lock)
        {
            while (_packetQueue.Count > 0)
                list.Add(_packetQueue.Dequeue());
        }

        return list;
    }
}