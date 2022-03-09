using System;
using FlatBuffers;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using GamePacket;

public class GamePacketMessage
{
    public Packet ID { get; set; }
    public Root Message { get; set; }
}

public class GamePacketQueue
{
    public static GamePacketQueue Instance { get; } = new GamePacketQueue();

    Queue<GamePacketMessage> _packetQueue = new Queue<GamePacketMessage>();
    object _lock = new object();

    public void Push(Packet id, Root packet)
    {
        lock (_lock)
        {
            _packetQueue.Enqueue(new GamePacketMessage() { ID = id, Message = packet });
        }
    }

    public GamePacketMessage Pop()
    {
        lock (_lock)
        {
            if (_packetQueue.Count == 0)
                return null;

            return _packetQueue.Dequeue();
        }
    }

    public List<GamePacketMessage> PopAll()
    {
        List<GamePacketMessage> list = new List<GamePacketMessage>();

        lock (_lock)
        {
            while (_packetQueue.Count > 0)
                list.Add(_packetQueue.Dequeue());
        }

        return list;
    }
}