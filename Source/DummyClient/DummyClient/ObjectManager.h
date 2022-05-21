#pragma once

#define OBJECT_MANAGER GET_INSTANCE(ObjectManager)

class ObjectManager
{
	DECLARE_SINGLETON(ObjectManager)

public:
	void AddPlayer(std::shared_ptr<CoreServerSession> session, const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo);
	void AddPlayer(const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo, const GamePacket::CharacterInfoT& characterInfo);

public:
	void RemoveObject(const NativeInfo::ObjectInfo& objectInfo);
	std::shared_ptr<Object> FindObject(const NativeInfo::ObjectInfo& objectInfo);

private:
	void RemovePlayer(const int64_t& oid);
	std::shared_ptr<Player> FindPlayer(const int64_t& oid);

public:
	void AddProjectile(const std::shared_ptr<ProjectileSkill> owner, const Info::ObjectInfoWithPosT& objectInfoWithPos);

private:
	void RemoveProjectile(const int64_t& oid);
	std::shared_ptr<Projectile> FindProjectile(const int64_t& oid);

private:
	CACHE_ALIGN std::shared_mutex playerMutex;
	std::map<int64_t, std::shared_ptr<Player>> playerList;

private:
	CACHE_ALIGN std::shared_mutex projectileMutex;
	std::map<int64_t, std::shared_ptr<Projectile>> projectileList;

};