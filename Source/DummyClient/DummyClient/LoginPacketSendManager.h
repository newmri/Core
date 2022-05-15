#pragma once

#define LOGIN_PACKET_SEND_MANAGER GET_INSTANCE(LoginPacketSendManager)

class LoginPacketSendManager
{
	DECLARE_SINGLETON(LoginPacketSendManager)

public:
	static void Clear(void);
	
public:
	static void Send(CoreServerSession& session, LoginPacket::Packet packetType, flatbuffers::Offset<void> packet);

public:
	static thread_local bool isFinished;
	static thread_local flatbuffers::FlatBufferBuilder builder;
};