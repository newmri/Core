#include "Include.h"

GamePacketHandler::GamePacketHandler()
{
    Register();
}

void GamePacketHandler::Handle(std::shared_ptr<CoreClientSession> session, const GamePacket::Packet packetID, const void* data)
{
	if (func.end() == func.find(packetID))
		return;

	func[packetID](this->packetFunc, session, data);
}

void GamePacketHandler::Register(void)
{
    this->func[GamePacket::Packet_CS_LOGIN_REQ] = &GamePacketFunc::CS_LOGIN_REQ;
    this->func[GamePacket::Packet_CS_PING_RES] = &GamePacketFunc::CS_PING_RES;
    this->func[GamePacket::Packet_CS_LOGOUT_NOTI] = &GamePacketFunc::CS_LOGOUT_NOTI;
    this->func[GamePacket::Packet_CS_MOVE_REQ] = &GamePacketFunc::CS_MOVE_REQ;
    this->func[GamePacket::Packet_CS_SET_STATE_REQ] = &GamePacketFunc::CS_SET_STATE_REQ;
    this->func[GamePacket::Packet_CS_USE_SKILL_REQ] = &GamePacketFunc::CS_USE_SKILL_REQ;
}
