#include "Include.h"

IMPLEMENT_SINGLETON(ObjectManager)

void ObjectManager::Init(void)
{
}

void ObjectManager::Release(void)
{
	GetInstance().~ObjectManager();
}

int64_t ObjectManager::AddPlayer(const int64_t& characterUID, std::shared_ptr<CoreClientSession> session,
	Info::ObjectInfoT& objectInfo, Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo)
{
	int64_t oid = this->oid.fetch_add(1);
	session->SetPlayerOID(oid);

	objectInfo.oid = oid;
	objectInfo.object_type = Define::ObjectType_PLAYER;
	CHARACTER_DATA_MANAGER.CalculateAbilityByStat(creatureInfo);
	CHARACTER_DATA_MANAGER.CalculateSpeed(characterInfo.job, creatureInfo.speed);

	auto player = std::make_shared<Player>(characterUID, session, objectInfo, creatureInfo, characterInfo);
	ZONE_MANAGER.EnterStartPos(objectInfo.pos_info.mapID, player);
	objectInfo.pos_info.pos = player->GetPos();
	player->AddSkill(static_cast<int32_t>(player->GetCharacterInfo().job));

	WRITE_LOCK(this->mutex);
	this->playerList[oid] = player;
	return oid;
}

std::shared_ptr<Player> ObjectManager::FindPlayer(const int64_t& oid)
{
	READ_LOCK(this->mutex);

	auto iter = this->playerList.find(oid);
	if (IS_NOT_SAME(iter, this->playerList.end()))
		return iter->second;

	return nullptr;
}

void ObjectManager::RemovePlayer(const int64_t& oid)
{
	auto player = FindPlayer(oid);
	if (IS_NULL(player))
		return;

	GAME_SERVER.GetGameDB()->Logout(player->GetUID());

	ZONE_MANAGER.Leave(player);

	WRITE_LOCK(this->mutex);
	this->playerList.erase(oid);
}