using UnityCoreLibrary;
using FlatBuffers;
using System;
using System.Collections.Generic;
using UnityEngine;
using GamePacket;

class GamePacketManager
{
	#region Singleton
	static GamePacketManager _instance = new GamePacketManager();
	public static GamePacketManager Instance { get { return _instance; } }
	#endregion

	GamePacketManager()
	{
		Register();
	}

    Dictionary<Packet, Action<PacketSession, Root>> _handler = new Dictionary<Packet, Action<PacketSession, Root>>();

    public Action<PacketSession, Packet, Root> CustomHandler { get; set; }

	public void Register()
	{
       _handler.Add(Packet.SC_LOGIN_RES, GamePacketHandler.SC_LOGIN_RES);
       _handler.Add(Packet.SC_ITEM_INVENTORY_INFO_NOTI, GamePacketHandler.SC_ITEM_INVENTORY_INFO_NOTI);
       _handler.Add(Packet.SC_PING_REQ, GamePacketHandler.SC_PING_REQ);
       _handler.Add(Packet.SC_SPAWN_PLAYER_NOTI, GamePacketHandler.SC_SPAWN_PLAYER_NOTI);
       _handler.Add(Packet.SC_DESPAWN_OBJECT_NOTI, GamePacketHandler.SC_DESPAWN_OBJECT_NOTI);
       _handler.Add(Packet.SC_MOVE_RES, GamePacketHandler.SC_MOVE_RES);
       _handler.Add(Packet.SC_SET_STATE_RES, GamePacketHandler.SC_SET_STATE_RES);
       _handler.Add(Packet.SC_USE_SKILL_RES, GamePacketHandler.SC_USE_SKILL_RES);
       _handler.Add(Packet.SC_GET_DAMAGE_NOTI, GamePacketHandler.SC_GET_DAMAGE_NOTI);
       _handler.Add(Packet.SC_REVIVE_RES, GamePacketHandler.SC_REVIVE_RES);
       _handler.Add(Packet.SC_SPAWN_PROJECTILE_NOTI, GamePacketHandler.SC_SPAWN_PROJECTILE_NOTI);
       _handler.Add(Packet.SC_EQUIP_GEAR_RES, GamePacketHandler.SC_EQUIP_GEAR_RES);
       _handler.Add(Packet.SC_UNEQUIP_GEAR_RES, GamePacketHandler.SC_UNEQUIP_GEAR_RES);
       _handler.Add(Packet.SC_ABILITY_INFO_NOTI, GamePacketHandler.SC_ABILITY_INFO_NOTI);
       _handler.Add(Packet.SC_NORMAL_CHAT_RES, GamePacketHandler.SC_NORMAL_CHAT_RES);
       _handler.Add(Packet.SC_ADDED_ITEM_TO_ITEM_INVENTORY_NOTI, GamePacketHandler.SC_ADDED_ITEM_TO_ITEM_INVENTORY_NOTI);
       _handler.Add(Packet.SC_ADD_STAT_RES, GamePacketHandler.SC_ADD_STAT_RES);
       _handler.Add(Packet.SC_MY_LEVEL_UP_NOTI, GamePacketHandler.SC_MY_LEVEL_UP_NOTI);
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