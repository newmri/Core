#include "Include.h"

IMPLEMENT_SINGLETON(CreatureManager)

void CreatureManager::Init(void)
{
}

void CreatureManager::Release(void)
{
	GetInstance().~CreatureManager();
}

int64_t CreatureManager::AddPlayer(const int64_t& characterUID, std::shared_ptr<CoreClientSession> session,
	Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo)
{
	int64_t oid = this->oid.fetch_add(1);
	session->SetPlayerOID(oid);

	creatureInfo.oid = oid;
	creatureInfo.object_type = Define::ObjectType_PLAYER;
	CHARACTER_DATA_MANAGER.CalculateAbilityByStat(creatureInfo);
	CHARACTER_DATA_MANAGER.CalculateSpeed(characterInfo.job, creatureInfo.speed);

	auto player = std::make_shared<Player>(characterUID, session, creatureInfo, characterInfo);
	ZONE_MANAGER.EnterStartPos(creatureInfo.pos_info.mapID, player);
	creatureInfo.pos_info.pos = player->GetPos();

	WRITE_LOCK(this->mutex);
	this->playerList[oid] = player;
	return oid;
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

	ZONE_MANAGER.Leave(player);

	WRITE_LOCK(this->mutex);
	this->playerList.erase(oid);
}