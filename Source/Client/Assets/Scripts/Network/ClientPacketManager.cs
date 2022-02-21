using UnityCoreLibrary;
using FlatBuffers;
using System;
using System.Collections.Generic;

class PacketManager
{
    #region Singleton
    static PacketManager _instance = new PacketManager();
    public static PacketManager Instance { get { return _instance; } }
    #endregion

    PacketManager()
    {
        Register();
    }

    Dictionary<ushort, Action<PacketSession, ArraySegment<byte>, ushort>> _onRecv = new Dictionary<ushort, Action<PacketSession, ArraySegment<byte>, ushort>>();
    Dictionary<ushort, Action<PacketSession, IFlatbufferObject>> _handler = new Dictionary<ushort, Action<PacketSession, IFlatbufferObject>>();

    public Action<PacketSession, IFlatbufferObject, ushort> CustomHandler { get; set; }

    public void Register()
    {
        _onRecv.Add((ushort)Login.Packet.SC_LOGIN_RES, MakePacket<Login.SC_LOGIN_RES>);
        _handler.Add((ushort)Login.Packet.SC_LOGIN_RES, PacketHandler.SC_LOGIN_RES);
    }

    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
    {
        ushort count = 0;

        int size = BitConverter.ToInt32(buffer.Array, buffer.Offset);
        count += sizeof(int);

       
        //ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
        //count += 2;

        //Action<PacketSession, ArraySegment<byte>, ushort> action = null;
        //if (_onRecv.TryGetValue(id, out action))
        //    action.Invoke(session, buffer, id);
    }

    void MakePacket<T>(PacketSession session, ArraySegment<byte> buffer, ushort id) where T : IFlatbufferObject, new()
    {
        //T pkt = new T();
        //pkt(buffer.Array, buffer.Offset + 4, buffer.Count - 4);

        //if (CustomHandler != null)
        //{
        //    CustomHandler.Invoke(session, pkt, id);
        //}
        //else
        //{
        //    Action<PacketSession, IFlatbufferObject> action = null;
        //    if (_handler.TryGetValue(id, out action))
        //        action.Invoke(session, pkt);
        //}
    }

    public Action<PacketSession, IFlatbufferObject> GetPacketHandler(ushort id)
    {
        Action<PacketSession, IFlatbufferObject> action = null;
        if (_handler.TryGetValue(id, out action))
            return action;
        return null;
    }
}