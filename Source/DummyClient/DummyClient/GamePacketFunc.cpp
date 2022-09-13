#include "Include.h"

void GamePacketFunc::SC_LOGIN_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
	auto raw = static_cast<const GamePacket::SC_LOGIN_RES*>(data);

	if (IS_SAME(GamePacket::ErrorCode_SUCCESS, raw->result()))
	{
		OBJECT_MANAGER.AddPlayer(session, *raw->object_info_with_pos()->UnPack(), *raw->creature_info()->UnPack(), *raw->character_info()->UnPack());

		CoreAccount* account = CORE_ACCOUNT_MANAGER.Find(session->GetAccountUID());
		if (IS_NULL(account))
			return;

		auto money = raw->money()->UnPack();
		for (int32_t i = 0; i < Define::MoneyType_END; ++i)
			account->PushMoney(money->value.value[i]);
	}
}

void GamePacketFunc::SC_ITEM_INVENTORY_INFO_NOTI(std::shared_ptr<CoreServerSession> session, const void* data)
{
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

	auto characterInfo = raw->character_info()->UnPack();
	if (DUMMY_CLIENT.IsMyPlayer(characterInfo->name))
		return;

	OBJECT_MANAGER.AddPlayer(*raw->object_info_with_pos()->UnPack(), *raw->creature_info()->UnPack(), *characterInfo);
}

void GamePacketFunc::SC_DESPAWN_OBJECT_NOTI(std::shared_ptr<CoreServerSession> session, const void* data)
{
	auto raw = static_cast<const GamePacket::SC_DESPAWN_OBJECT_NOTI*>(data);
	auto unpakcedObjectInfo = flatbuffers::UnPackObjectInfo(*raw->object_info());
	OBJECT_MANAGER.RemoveObject(unpakcedObjectInfo);
}

void GamePacketFunc::SC_MOVE_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
	auto raw = static_cast<const GamePacket::SC_MOVE_RES*>(data);
	auto unpakcedObjectInfoWithPos = raw->object_info_with_pos()->UnPack();
	auto object = OBJECT_MANAGER.FindObject(unpakcedObjectInfoWithPos->object_info);
	if (IS_NULL(object))
		return;

	ZONE_MANAGER.Move(object, *unpakcedObjectInfoWithPos);
}

void GamePacketFunc::SC_SET_STATE_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
	auto raw = static_cast<const GamePacket::SC_SET_STATE_RES*>(data);
	auto unpakcedObjectInfo = flatbuffers::UnPackObjectInfo(*raw->object_info());
	auto object = OBJECT_MANAGER.FindObject(unpakcedObjectInfo);
	if (IS_NULL(object))
		return;

	object->SetState(raw->state());
}

void GamePacketFunc::SC_USE_SKILL_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
	auto raw = static_cast<const GamePacket::SC_USE_SKILL_RES*>(data);
	auto unpakcedObjectInfo = flatbuffers::UnPackObjectInfo(*raw->object_info());
	auto object = OBJECT_MANAGER.FindObject(unpakcedObjectInfo);
	if (IS_NULL(object))
		return;

	object->SetState(Define::ObjectState_SKILL);
}

void GamePacketFunc::SC_GET_DAMAGE_NOTI(std::shared_ptr<CoreServerSession> session, const void* data)
{
	auto raw = static_cast<const GamePacket::SC_GET_DAMAGE_NOTI*>(data);
	auto infoList = raw->damage_info();
	if (IS_NULL(infoList))
		return;

	auto iter_begin = infoList->begin();
	auto iter_end = infoList->end();
	std::shared_ptr<Object> object;
	GamePacket::DamageInfoT damageInfo;
	for (; iter_begin != iter_end; ++iter_begin)
	{
		damageInfo = *iter_begin->UnPack();

		object = OBJECT_MANAGER.FindObject(damageInfo.object_info);
		if (IS_NULL(object))
			return;

		object->OnGetDamage(damageInfo);
	}
}

void GamePacketFunc::SC_REVIVE_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
	auto raw = static_cast<const GamePacket::SC_REVIVE_RES*>(data);
	auto unpakcedObjectInfoWithPos = raw->object_info_with_pos()->UnPack();
	auto creature = OBJECT_MANAGER.FindCreature(unpakcedObjectInfoWithPos->object_info);
	if (IS_NULL(creature))
		return;

	creature->Revive(*unpakcedObjectInfoWithPos);
}

void GamePacketFunc::SC_SPAWN_PROJECTILE_NOTI(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_EQUIP_GEAR_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_UNEQUIP_GEAR_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_ABILITY_INFO_NOTI(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_NORMAL_CHAT_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_ADDED_ITEM_TO_ITEM_INVENTORY_NOTI(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_ADD_STAT_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_MY_LEVEL_UP_NOTI(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_LEVEL_UP_NOTI(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_USE_ITEM_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
}

void GamePacketFunc::SC_ADDED_EFFECT_NOTI(std::shared_ptr<CoreServerSession> session, const void* data)
{
}
