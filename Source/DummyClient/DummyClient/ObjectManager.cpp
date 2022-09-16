#include "Include.h"

IMPLEMENT_SINGLETON(ObjectManager)

void ObjectManager::Init(void)
{
}

void ObjectManager::Release(void)
{
	GetInstance().~ObjectManager();
}

void ObjectManager::AddPlayer(std::shared_ptr<CoreServerSession> session, const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo)
{
	session->SetPlayerOID(objectInfoWithPos.object_info.oid);

	auto player = std::make_shared<MyPlayer>(session, objectInfoWithPos, creatureInfo, characterInfo);
	player->AddSkill(static_cast<int32_t>(player->GetCharacterInfo().job));

	WRITE_LOCK(this->playerMutex);
	this->playerList[objectInfoWithPos.object_info.oid] = player;
}

void ObjectManager::AddPlayer(const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo, const GamePacket::CharacterInfoT& characterInfo)
{
	auto player = std::make_shared<OtherPlayer>(objectInfoWithPos, creatureInfo, characterInfo);

	WRITE_LOCK(this->playerMutex);
	this->playerList[objectInfoWithPos.object_info.oid] = player;
}

void ObjectManager::RemoveObject(const NativeInfo::ObjectInfo& objectInfo)
{
	switch (objectInfo.objectType)
	{
	case Define::ObjectType_PLAYER:
		RemovePlayer(objectInfo.oid);
		break;
	case Define::ObjectType_MONSTER:
		break;
	case Define::ObjectType_PROJECTILE:
		RemoveProjectile(objectInfo.oid);
		break;
	default:
		break;
	}
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

std::shared_ptr<Creature> ObjectManager::FindCreature(const NativeInfo::ObjectInfo& objectInfo)
{
	switch (objectInfo.objectType)
	{
	case Define::ObjectType_PLAYER:
		return FindPlayer(objectInfo.oid);
	case Define::ObjectType_MONSTER:
		break;
	default:
		return nullptr;
	}

	return nullptr;
}

void ObjectManager::RemovePlayer(const int64_t& oid)
{
	auto player = FindPlayer(oid);
	if (IS_NULL(player))
		return;

	player->Clear();

	WRITE_LOCK(this->playerMutex);
	this->playerList.erase(oid);
}

std::shared_ptr<Player> ObjectManager::FindPlayer(const int64_t& oid)
{
	READ_LOCK(this->playerMutex);

	auto iter = this->playerList.find(oid);
	if (IS_NOT_SAME(iter, this->playerList.end()))
		return iter->second;

	return nullptr;
}

std::shared_ptr<MyPlayer> ObjectManager::FindMyPlayer(const int64_t& oid)
{
	READ_LOCK(this->playerMutex);

	auto iter = this->playerList.find(oid);
	if (IS_NOT_SAME(iter, this->playerList.end()))
		return std::static_pointer_cast<MyPlayer>(iter->second);

	return nullptr;
}

void ObjectManager::AddProjectile(const std::shared_ptr<ProjectileSkill> owner, const Info::ObjectInfoWithPosT& objectInfoWithPos)
{
	std::shared_ptr<Projectile> projectile = nullptr;
	switch (owner->GetSkillType())
	{
	case Define::SkillType_ARROW:
		projectile = std::make_shared<Arrow>(owner, objectInfoWithPos);
		break;
	default:
		return;
	}

	WRITE_LOCK(this->projectileMutex);
	this->projectileList[objectInfoWithPos.object_info.oid] = projectile;
}

void ObjectManager::RemoveProjectile(const int64_t& oid)
{
	auto projectile = FindProjectile(oid);
	if (IS_NULL(projectile))
		return;

	WRITE_LOCK(this->projectileMutex);
	this->projectileList.erase(oid);
}

std::shared_ptr<Projectile> ObjectManager::FindProjectile(const int64_t& oid)
{
	READ_LOCK(this->projectileMutex);

	auto iter = this->projectileList.find(oid);
	if (IS_NOT_SAME(iter, this->projectileList.end()))
		return iter->second;

	return nullptr;
}