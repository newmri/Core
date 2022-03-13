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
	int64_t uid = this->uid.fetch_add(1);
	session->SetPlayerUID(uid);

	creatureInfo.uid = uid;
	creatureInfo.obj_type = Define::ObjectType_PLAYER;
	DATA_MANAGER.CalculateAbilityByStat(creatureInfo);
	auto character = std::make_shared<Character>(creatureInfo, characterInfo);

	WRITE_LOCK(this->mutex);
	this->playerList[uid] = std::make_shared<Player>(uid, session, character);
}

std::shared_ptr<Player> CreatureManager::FindPlayer(const int64_t& uid)
{
	READ_LOCK(this->mutex);

	auto iter = this->playerList.find(uid);
	if (IS_NOT_SAME(iter, this->playerList.end()))
		return iter->second;

	return nullptr;
}

void CreatureManager::RemovePlayer(const int64_t& uid)
{
	auto player = FindPlayer(uid);
	if (IS_NULL(player))
		return;

	WRITE_LOCK(this->mutex);
	this->playerList.erase(uid);
}