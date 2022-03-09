using UnityCoreLibrary;
using FlatBuffers;
using System;
using System.Collections.Generic;
using UnityEngine;
using LoginPacket;

class LoginPacketPacketManager
{
	#region Singleton
	static LoginPacketPacketManager _instance = new LoginPacketPacketManager();
	public static LoginPacketPacketManager Instance { get { return _instance; } }
	#endregion

	LoginPacketPacketManager()
	{
		Register();
	}

    Dictionary<Packet, Action<PacketSession, Root>> _handler = new Dictionary<Packet, Action<PacketSession, Root>>();

    public Action<PacketSession, Packet, Root> CustomHandler { get; set; }

	public void Register()
	{
       _handler.Add(Packet.SC_LOGIN_RES, LoginPacketPacketHandler.SC_LOGIN_RES);
       _handler.Add(Packet.SC_PING_REQ, LoginPacketPacketHandler.SC_PING_REQ);
       _handler.Add(Packet.SC_CREATE_CHARACTER_RES, LoginPacketPacketHandler.SC_CREATE_CHARACTER_RES);
	}

    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
    {
        ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);

        byte[] recvBuffer = new byte[size];
        Array.Copy(buffer.Array, PackeSize.HEADER_SIZE, recvBuffer, 0, size);
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