using UnityCoreLibrary;
using FlatBuffers;
using System;
using System.Collections.Generic;
using UnityEngine;
using LoginPacket;

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
       _handler.Add(Packet.SC_PING_REQ, LoginPacketHandler.SC_PING_REQ);
       _handler.Add(Packet.SC_CREATE_CHARACTER_RES, LoginPacketHandler.SC_CREATE_CHARACTER_RES);
       _handler.Add(Packet.SC_TEST_RES, LoginPacketHandler.SC_TEST_RES);
	}

    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
    {
        int size = buffer.Count;
        byte[] recvBuffer = new byte[size];
        Array.Copy(buffer.Array, buffer.Offset + PackeSize.HEADER_SIZE, recvBuffer, 0, size);
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