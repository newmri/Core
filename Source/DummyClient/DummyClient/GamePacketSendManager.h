#pragma once

#define GAME_PACKET_SEND_MANAGER GET_INSTANCE(GamePacketSendManager)

class GamePacketSendManager
{
	DECLARE_SINGLETON(GamePacketSendManager)

public:
	static void Clear(void);

public:
	static void Send(std::shared_ptr<CoreServerSession> session, GamePacket::Packet packetType, flatbuffers::Offset<void> packet);

public:
	static thread_local bool isFinished;
	static thread_local flatbuffers::FlatBufferBuilder builder;
};