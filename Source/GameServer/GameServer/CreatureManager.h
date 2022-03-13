#pragma once

#define CREATURE_MANAGER GET_INSTANCE(CreatureManager)

class CreatureManager
{
	DECLARE_SINGLETON(CreatureManager)

public:
	void AddPlayer(const int64_t& characterUID, std::shared_ptr<CoreClientSession> session,
		Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo);

	std::shared_ptr<Player> FindPlayer(const int64_t& uid);

	void RemovePlayer(const int64_t& uid);

private:
	std::atomic<int64_t> uid;

private:
	CACHE_ALIGN std::shared_mutex mutex;
	std::map<int64_t, std::shared_ptr<Player>> playerList;
};