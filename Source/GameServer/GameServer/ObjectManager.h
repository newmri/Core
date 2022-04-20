#pragma once

#define OBJECT_MANAGER GET_INSTANCE(ObjectManager)

class ObjectManager
{
	DECLARE_SINGLETON(ObjectManager)

public:
	int64_t AddPlayer(const int64_t& characterUID, std::shared_ptr<CoreClientSession> session,
		Info::ObjectInfoT& objectInfo, Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo);

	std::shared_ptr<Player> FindPlayer(const int64_t& oid);

	void RemovePlayer(const int64_t& oid);

private:
	std::atomic<int64_t> oid;

private:
	CACHE_ALIGN std::shared_mutex mutex;
	std::map<int64_t, std::shared_ptr<Player>> playerList;
};