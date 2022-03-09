#include "Include.h"

GamePacketPacketHandler::GamePacketPacketHandler()
{
    Register();
}

void GamePacketPacketHandler::Handle(std::shared_ptr<CoreClientSession> session, const GamePacket::Packet packetID, const void* data)
{
	if (func.end() == func.find(packetID))
		return;

	func[packetID](this->packetFunc, session, data);
}

void GamePacketPacketHandler::Register(void)
{
    this->func[GamePacket::Packet_CS_LOGIN_REQ] = &GamePacketPacketFunc::CS_LOGIN_REQ;
    this->func[GamePacket::Packet_CS_PING_RES] = &GamePacketPacketFunc::CS_PING_RES;
    this->func[GamePacket::Packet_CS_LOGOUT_NOTI] = &GamePacketPacketFunc::CS_LOGOUT_NOTI;
}
