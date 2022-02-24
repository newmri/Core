using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
	class PacketFormatForCS
	{
        public static string FileName =
@"{0}PacketManager.cs";
        
        public static string managerFormat =
@"using UnityCoreLibrary;
using FlatBuffers;
using System;
using System.Collections.Generic;
using UnityEngine;
using {0};

class {0}PacketManager
{{
	#region Singleton
	static {0}PacketManager _instance = new {0}PacketManager();
	public static {0}PacketManager Instance {{ get {{ return _instance; }} }}
	#endregion

	{0}PacketManager()
	{{
		Register();
	}}

    Dictionary<Packet, Action<PacketSession, Root>> _handler = new Dictionary<Packet, Action<PacketSession, Root>>();

    public Action<PacketSession, Packet, Root> CustomHandler {{ get; set; }}

	public void Register()
	{{{1}
	}}

    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
    {{
        ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);

        byte[] recvBuffer = new byte[size];
        Array.Copy(buffer.Array, PackeSize.HEADER_SIZE, recvBuffer, 0, size);
        ByteBuffer byteBuffer = new ByteBuffer(recvBuffer);

        Root root = Root.GetRootAsRoot(byteBuffer);

        if (CustomHandler != null)
        {{
            CustomHandler.Invoke(session, root.PacketType, root);
        }}
        else
        {{
            Action<PacketSession, Root> action = null;
            if (_handler.TryGetValue(root.PacketType, out action))
                action.Invoke(session, root);
        }}
    }}

    public Action<PacketSession, Root> GetPacketHandler(Packet id)
    {{
        Action<PacketSession, Root> action = null;
        if (_handler.TryGetValue(id, out action))
            return action;
        return null;
    }}
}}";

		public static string managerRegisterFormat =
@"_handler.Add(Packet.{0}, {1}PacketHandler.{0});";
	}
}
