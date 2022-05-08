#include "Include.h"

GamePacketHandler::GamePacketHandler()
{
    Register();
}

void GamePacketHandler::Handle(CoreServerSession& session, const GamePacket::Packet packetID, const void* data)
{
	if (this->func.end() == this->func.find(packetID))
		return;

	this->func[packetID](this->packetFunc, session, data);
}

void GamePacketHandler::Register(void)
{
    this->func[GamePacket::Packet_SC_LOGIN_RES] = &GamePacketFunc::SC_LOGIN_RES;
    this->func[GamePacket::Packet_SC_PING_REQ] = &GamePacketFunc::SC_PING_REQ;
    this->func[GamePacket::Packet_SC_SPAWN_PLAYER_NOTI] = &GamePacketFunc::SC_SPAWN_PLAYER_NOTI;
    this->func[GamePacket::Packet_SC_DESPAWN_OBJECT_NOTI] = &GamePacketFunc::SC_DESPAWN_OBJECT_NOTI;
    this->func[GamePacket::Packet_SC_MOVE_RES] = &GamePacketFunc::SC_MOVE_RES;
    this->func[GamePacket::Packet_SC_SET_STATE_RES] = &GamePacketFunc::SC_SET_STATE_RES;
    this->func[GamePacket::Packet_SC_USE_SKILL_RES] = &GamePacketFunc::SC_USE_SKILL_RES;
    this->func[GamePacket::Packet_SC_GET_DAMAGE_NOTI] = &GamePacketFunc::SC_GET_DAMAGE_NOTI;
    this->func[GamePacket::Packet_SC_REVIVE_RES] = &GamePacketFunc::SC_REVIVE_RES;
    this->func[GamePacket::Packet_SC_SPAWN_PROJECTILE_NOTI] = &GamePacketFunc::SC_SPAWN_PROJECTILE_NOTI;
}
