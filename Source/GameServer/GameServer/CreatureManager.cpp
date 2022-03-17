#include "Include.h"

IMPLEMENT_SINGLETON(CreatureManager)

void CreatureManager::Init(void)
{
}

void CreatureManager::Release(void)
{
	GetInstance().~CreatureManager();
}

void CreatureManager::AddPlayer(const int64_t& characterUID, std::shared_ptr<CoreClientSession> session,
	Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo)
{
	int64_t oid = this->oid.fetch_add(1);
	session->SetPlayerOID(oid);

	creatureInfo.oid = oid;
	creatureInfo.obj_type = Define::ObjectType_PLAYER;
	CHARACTER_DATA_MANAGER.CalculateAbilityByStat(creatureInfo);
	CHARACTER_DATA_MANAGER.CalculateSpeed(characterInfo.job, creatureInfo.speed);
	ZONE_MANAGER.EnterStartPos(creatureInfo.pos_info.mapID, creatureInfo.obj_type, creatureInfo.oid, creatureInfo.pos_info.pos);

	WRITE_LOCK(this->mutex);
	this->playerList[oid] = std::make_shared<Player>(characterUID, session, creatureInfo, characterInfo);
}

std::shared_ptr<Player> CreatureManager::FindPlayer(const int64_t& oid)
{
	READ_LOCK(this->mutex);

	auto iter = this->playerList.find(oid);
	if (IS_NOT_SAME(iter, this->playerList.end()))
		return iter->second;

	return nullptr;
}

void CreatureManager::RemovePlayer(const int64_t& oid)
{
	auto player = FindPlayer(oid);
	if (IS_NULL(player))
		return;

	ZONE_MANAGER.Leave(player->GetMapID(), player->GetObjectType(), oid, player->GetPos());

	WRITE_LOCK(this->mutex);
	this->playerList.erase(oid);
}