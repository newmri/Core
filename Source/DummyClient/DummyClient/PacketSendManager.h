#pragma once

#define PACKET_SEND_MANAGER GET_INSTANCE(PacketSendManager)

class PacketSendManager
{
	DECLARE_SINGLETON(PacketSendManager)

public:
	static void Send(CoreServerSession& session, LoginPacket::Packet packetType, flatbuffers::Offset<void> packet);
	static void Send(CoreServerSession& session, GamePacket::Packet packetType, flatbuffers::Offset<void> packet);

public:
	static thread_local flatbuffers::FlatBufferBuilder builder;
};