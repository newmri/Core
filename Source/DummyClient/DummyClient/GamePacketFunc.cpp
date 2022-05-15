#include "Include.h"

void GamePacketFunc::SC_LOGIN_RES(CoreServerSession& session, const void* data)
{
	auto raw = static_cast<const LoginPacket::SC_LOGIN_RES*>(data);

	if (IS_SAME(LoginPacket::ErrorCode_SUCCESS, raw->result()))
	{
		
	}
}

void GamePacketFunc::SC_PING_REQ(CoreServerSession& session, const void* data)
{
	GAME_PACKET_SEND_MANAGER.Clear();
	auto message = GamePacket::CreateCS_PING_RES(GAME_PACKET_SEND_MANAGER.builder);
	GAME_PACKET_SEND_MANAGER.Send(session, GamePacket::Packet_CS_PING_RES, message.Union());
}

void GamePacketFunc::SC_SPAWN_PLAYER_NOTI(CoreServerSession& session, const void* data)
{
}

void GamePacketFunc::SC_DESPAWN_OBJECT_NOTI(CoreServerSession& session, const void* data)
{
}

void GamePacketFunc::SC_MOVE_RES(CoreServerSession& session, const void* data)
{
}

void GamePacketFunc::SC_SET_STATE_RES(CoreServerSession& session, const void* data)
{
}

void GamePacketFunc::SC_USE_SKILL_RES(CoreServerSession& session, const void* data)
{
}

void GamePacketFunc::SC_GET_DAMAGE_NOTI(CoreServerSession& session, const void* data)
{
}

void GamePacketFunc::SC_REVIVE_RES(CoreServerSession& session, const void* data)
{
}

void GamePacketFunc::SC_SPAWN_PROJECTILE_NOTI(CoreServerSession& session, const void* data)
{
}
