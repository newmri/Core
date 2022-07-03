#pragma once

#define OBJECT_MANAGER GET_INSTANCE(ObjectManager)

class ObjectManager
{
	DECLARE_SINGLETON(ObjectManager)

public:
	std::shared_ptr<Player> AddPlayer(const int64_t& characterUID, std::shared_ptr<CoreClientSession> session,
		Info::ObjectInfoWithPosT& objectInfoWithPos, Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo);

	std::shared_ptr<Player> FindPlayer(const int64_t& oid);

	void RemovePlayer(const int64_t& oid);

public:
	void AddProjectile(const std::shared_ptr<ProjectileSkill> owner, Info::ObjectInfoWithPosT& objectInfoWithPos);
	std::shared_ptr<Projectile> FindProjectile(const int64_t& oid);
	void RemoveProjectile(const int64_t& oid);

public:
	std::shared_ptr<Object> FindObject(const NativeInfo::ObjectInfo& objectInfo);

private:
	std::atomic<int64_t> oid;

private:
	CACHE_ALIGN std::shared_mutex playerMutex;
	std::map<int64_t, std::shared_ptr<Player>> playerList;

private:
	CACHE_ALIGN std::shared_mutex projectileMutex;
	std::map<int64_t, std::shared_ptr<Projectile>> projectileList;

};