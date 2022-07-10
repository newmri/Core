#include "Include.h"

GamePacketHandler::GamePacketHandler()
{
    Register();
}

void GamePacketHandler::Handle(std::shared_ptr<CoreClientSession> session, const GamePacket::Packet packetID, const void* data)
{
	if (this->func.end() == this->func.find(packetID))
		return;

	this->func[packetID](this->packetFunc, session, data);
}

void GamePacketHandler::Register(void)
{
    this->func[GamePacket::Packet_CS_LOGIN_REQ] = &GamePacketFunc::CS_LOGIN_REQ;
    this->func[GamePacket::Packet_CS_PING_RES] = &GamePacketFunc::CS_PING_RES;
    this->func[GamePacket::Packet_CS_LOGOUT_NOTI] = &GamePacketFunc::CS_LOGOUT_NOTI;
    this->func[GamePacket::Packet_CS_MOVE_REQ] = &GamePacketFunc::CS_MOVE_REQ;
    this->func[GamePacket::Packet_CS_SET_STATE_REQ] = &GamePacketFunc::CS_SET_STATE_REQ;
    this->func[GamePacket::Packet_CS_USE_SKILL_REQ] = &GamePacketFunc::CS_USE_SKILL_REQ;
    this->func[GamePacket::Packet_CS_REVIVE_REQ] = &GamePacketFunc::CS_REVIVE_REQ;
    this->func[GamePacket::Packet_CS_EQUIP_GEAR_REQ] = &GamePacketFunc::CS_EQUIP_GEAR_REQ;
    this->func[GamePacket::Packet_CS_UNEQUIP_GEAR_REQ] = &GamePacketFunc::CS_UNEQUIP_GEAR_REQ;
    this->func[GamePacket::Packet_CS_NORMAL_CHAT_REQ] = &GamePacketFunc::CS_NORMAL_CHAT_REQ;
}
