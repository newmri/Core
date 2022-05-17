#include "Include.h"

void GamePacketFunc::SC_LOGIN_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
	auto raw = static_cast<const GamePacket::SC_LOGIN_RES*>(data);

	if (IS_SAME(GamePacket::ErrorCode_SUCCESS, raw->result()))
	{
		OBJECT_MANAGER.AddPlayer(session, *raw->object_info()->UnPack(), *raw->creature_info()->UnPack(), *raw->character_info()->UnPack());

		CoreAccount* account = CORE_ACCOUNT_MANAGER.Find(session->GetAccountUID());
		if (IS_NULL(account))
			return;

		auto money = raw->money()->UnPack();
		for (int32_t i = 0; i < Define::MoneyType_END; ++i)
			account->PushMoney(money->value.value[i]);
	}
}

void GamePacketFunc::SC_PING_REQ(std::shared_ptr<CoreServerSession> session, const void* data)
{
	GAME_PACKET_SEND_MANAGER.Clear();
	auto message = GamePacket::CreateCS_PING_RES(GAME_PACKET_SEND_MANAGER.builder);
	GAME_PACKET_SEND_MANAGER.Send(session, GamePacket::Packet_CS_PING_RES, message.Union());
}

void GamePacketFunc::SC_SPAWN_PLAYER_NOTI(std::shared_ptr<CoreServerSession> session, const void* data)
{
	auto raw = static_cast<const GamePacket::SC_SPAWN_PLAYER_NOTI*>(data);

	OBJECT_MANAGER.AddPlayer(session, *raw->object_info()->UnPack(), *raw->creature_info()->UnPack(), *raw->character_info()->UnPack());
}

void GamePacketFunc::SC_DESPAWN_OBJECT_NOTI(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_MOVE_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_SET_STATE_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_USE_SKILL_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_GET_DAMAGE_NOTI(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_REVIVE_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_SPAWN_PROJECTILE_NOTI(std::shared_ptr<CoreServerSession> session, const void* data)
{
}
