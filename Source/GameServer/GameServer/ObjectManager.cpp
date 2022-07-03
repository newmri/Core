#include "Include.h"

IMPLEMENT_SINGLETON(ObjectManager)

void ObjectManager::Init(void)
{
}

void ObjectManager::Release(void)
{
	GetInstance().~ObjectManager();
}

std::shared_ptr<Player> ObjectManager::AddPlayer(const int64_t& characterUID, std::shared_ptr<CoreClientSession> session,
	Info::ObjectInfoWithPosT& objectInfoWithPos, Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo)
{
	int64_t oid = this->oid.fetch_add(1);
	session->SetPlayerOID(oid);

	objectInfoWithPos.object_info.oid = oid;
	objectInfoWithPos.object_info.objectType = Define::ObjectType_PLAYER;

	auto player = std::make_shared<Player>(characterUID, session, objectInfoWithPos, creatureInfo, characterInfo);
	ZONE_MANAGER.EnterStartPos(objectInfoWithPos.pos_info.mapID, player);
	objectInfoWithPos.pos_info.pos = player->GetPos();
	player->AddSkill(static_cast<int32_t>(player->GetCharacterInfo().job));
	creatureInfo = player->GetCreatureInfo();

	if (!player->LoadData())
	{
		this->oid.fetch_sub(1);
		return nullptr;
	}

	WRITE_LOCK(this->playerMutex);
	this->playerList[oid] = player;
	return player;
}

std::shared_ptr<Player> ObjectManager::FindPlayer(const int64_t& oid)
{
	READ_LOCK(this->playerMutex);

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

	ZONE_MANAGER.Leave(player);

	player->Clear();

	WRITE_LOCK(this->playerMutex);
	this->playerList.erase(oid);
}

void ObjectManager::AddProjectile(const std::shared_ptr<ProjectileSkill> owner, Info::ObjectInfoWithPosT& objectInfoWithPos)
{
	int64_t oid = this->oid.fetch_add(1);

	objectInfoWithPos.object_info.oid = oid;
	objectInfoWithPos.object_info.objectType = Define::ObjectType_PROJECTILE;
	objectInfoWithPos.pos_info.state = Define::ObjectState_WALK;

	std::shared_ptr<Projectile> projectile = nullptr;
	switch (owner->GetSkillType())
	{
	case Define::SkillType_ARROW:
		projectile = std::make_shared<Arrow>(owner, objectInfoWithPos);
		break;
	default:
		return;
	}

	ZONE_MANAGER.Enter(objectInfoWithPos.pos_info.mapID, projectile, false);

	projectile->PushMove();

	WRITE_LOCK(this->projectileMutex);
	this->projectileList[oid] = projectile;
}

std::shared_ptr<Projectile> ObjectManager::FindProjectile(const int64_t& oid)
{
	READ_LOCK(this->projectileMutex);

	auto iter = this->projectileList.find(oid);
	if (IS_NOT_SAME(iter, this->projectileList.end()))
		return iter->second;

	return nullptr;
}

void ObjectManager::RemoveProjectile(const int64_t& oid)
{
	auto projectile = FindProjectile(oid);
	if (IS_NULL(projectile))
		return;

	ZONE_MANAGER.Leave(projectile);

	WRITE_LOCK(this->projectileMutex);
	this->projectileList.erase(oid);
}

std::shared_ptr<Object> ObjectManager::FindObject(const NativeInfo::ObjectInfo& objectInfo)
{
	switch (objectInfo.objectType)
	{
	case Define::ObjectType_PLAYER:
		return FindPlayer(objectInfo.oid);
	case Define::ObjectType_MONSTER:
		break;
	case Define::ObjectType_PROJECTILE:
		return FindProjectile(objectInfo.oid);
	default:
		return nullptr;
	}

	return nullptr;
}
