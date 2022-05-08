#include "Include.h"

void LoginPacketFunc::SC_LOGIN_RES(CoreServerSession& session, const void* data)
{
}

void LoginPacketFunc::SC_PING_REQ(CoreServerSession& session, const void* data)
{
	PACKET_SEND_MANAGER.builder.Clear();
	auto message = LoginPacket::CreateCS_PING_RES(PACKET_SEND_MANAGER.builder);
	PACKET_SEND_MANAGER.Send(session, LoginPacket::Packet_CS_PING_RES, message.Union());
}

void LoginPacketFunc::SC_CREATE_CHARACTER_RES(CoreServerSession& session, const void* data)
{
}
