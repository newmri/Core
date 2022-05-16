#pragma once

#define OBJECT_MANAGER GET_INSTANCE(ObjectManager)

class ObjectManager
{
	DECLARE_SINGLETON(ObjectManager)

public:
	void AddPlayer(CoreServerSession& session, const Info::ObjectInfoT& objectInfo, const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo);

	std::shared_ptr<Player> FindPlayer(const int64_t& oid);

	void RemovePlayer(const int64_t& oid);

//public:
//	void AddProjectile(const std::shared_ptr<ProjectileSkill> owner, Info::ObjectInfoT& objectInfo);
//	std::shared_ptr<Projectile> FindProjectile(const int64_t& oid);
//	void RemoveProjectile(const int64_t& oid);

public:
	std::shared_ptr<Object> FindObject(ObjectInfo& objectInfo);

private:
	CACHE_ALIGN std::shared_mutex playerMutex;
	std::map<int64_t, std::shared_ptr<Player>> playerList;
//
//private:
//	CACHE_ALIGN std::shared_mutex projectileMutex;
//	std::map<int64_t, std::shared_ptr<Projectile>> projectileList;

};