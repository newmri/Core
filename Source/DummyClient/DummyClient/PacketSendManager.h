#pragma once

#define PACKET_SEND_MANAGER GET_INSTANCE(PacketSendManager)

class PacketSendManager
{
	DECLARE_SINGLETON(PacketSendManager)

public:
	static void Send(std::shared_ptr<CoreClientSession> session, LoginPacket::Packet packetType, flatbuffers::Offset<void> packet);

public:
	static thread_local flatbuffers::FlatBufferBuilder builder;
};