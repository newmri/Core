#include "Include.h"

IMPLEMENT_SINGLETON(ObjectManager)

void ObjectManager::Init(void)
{
}

void ObjectManager::Release(void)
{
	GetInstance().~ObjectManager();
}

void ObjectManager::AddPlayer(CoreServerSession& session, const Info::ObjectInfoT& objectInfo, const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo)
{
	session.SetPlayerOID(objectInfo.oid);

	auto player = std::make_shared<Player>(session, objectInfo, creatureInfo, characterInfo);
	player->AddSkill(static_cast<int32_t>(player->GetCharacterInfo().job));

	WRITE_LOCK(this->playerMutex);
	this->playerList[objectInfo.oid] = player;
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

	WRITE_LOCK(this->playerMutex);
	this->playerList.erase(oid);
}

//void ObjectManager::AddProjectile(const std::shared_ptr<ProjectileSkill> owner, Info::ObjectInfoT& objectInfo)
//{
//	int64_t oid = this->oid.fetch_add(1);
//
//	objectInfo.oid = oid;
//	objectInfo.object_type = Define::ObjectType_PROJECTILE;
//	objectInfo.pos_info.state = Define::ObjectState_WALK;
//
//	std::shared_ptr<Projectile> projectile = nullptr;
//	switch (owner->GetSkillType())
//	{
//	case Define::SkillType_ARROW:
//		projectile = std::make_shared<Arrow>(owner, objectInfo);
//		break;
//	default:
//		return;
//	}
//
//	ZONE_MANAGER.Enter(objectInfo.pos_info.mapID, projectile, false);
//
//	WRITE_LOCK(this->projectileMutex);
//	this->projectileList[oid] = projectile;
//}
//
//std::shared_ptr<Projectile> ObjectManager::FindProjectile(const int64_t& oid)
//{
//	READ_LOCK(this->projectileMutex);
//
//	auto iter = this->projectileList.find(oid);
//	if (IS_NOT_SAME(iter, this->projectileList.end()))
//		return iter->second;
//
//	return nullptr;
//}
//
//void ObjectManager::RemoveProjectile(const int64_t& oid)
//{
//	auto projectile = FindProjectile(oid);
//	if (IS_NULL(projectile))
//		return;
//
//	ZONE_MANAGER.Leave(projectile);
//
//	WRITE_LOCK(this->projectileMutex);
//	this->projectileList.erase(oid);
//}

std::shared_ptr<Object> ObjectManager::FindObject(ObjectInfo& objectInfo)
{
	switch (objectInfo.objectType)
	{
	case Define::ObjectType_PLAYER:
		return FindPlayer(objectInfo.oid);
	case Define::ObjectType_MONSTER:
		break;
	case Define::ObjectType_PROJECTILE:
		//return FindProjectile(objectInfo.oid);
	default:
		return nullptr;
	}

	return nullptr;
}
