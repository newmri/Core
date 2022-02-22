using UnityCoreLibrary;
using FlatBuffers;
using System;
using System.Collections.Generic;
using UnityEngine;
using Login;

class LoginPacketManager
{
    #region Singleton
    static LoginPacketManager _instance = new LoginPacketManager();
    public static LoginPacketManager Instance { get { return _instance; } }
    #endregion

    LoginPacketManager()
    {
        Register();
    }

    Dictionary<Packet, Action<PacketSession, Root>> _handler = new Dictionary<Packet, Action<PacketSession, Root>>();

    public Action<PacketSession, Packet, Root> CustomHandler { get; set; }

    public void Register()
    {
        _handler.Add(Packet.SC_LOGIN_RES, LoginPacketHandler.SC_LOGIN_RES);
    }

    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
    {
        ushort count = 0;

        int size = BitConverter.ToInt32(buffer.Array, buffer.Offset);
        count += sizeof(int);

        byte[] recvBuffer = new byte[size];
        Array.Copy(buffer.Array, sizeof(int), recvBuffer, 0, size);
        ByteBuffer byteBuffer = new ByteBuffer(recvBuffer);

        Root root = Root.GetRootAsRoot(byteBuffer);

        if (CustomHandler != null)
        {
            CustomHandler.Invoke(session, root.PacketType, root);
        }
        else
        {
            Action<PacketSession, Root> action = null;
            if (_handler.TryGetValue(root.PacketType, out action))
                action.Invoke(session, root);
        }
    }

    public Action<PacketSession, Root> GetPacketHandler(Packet id)
    {
        Action<PacketSession, Root> action = null;
        if (_handler.TryGetValue(id, out action))
            return action;
        return null;
    }
}