#pragma once

class Sector : CoreObject
{
	OVERRIDE_OBJECT(Sector)

public:
	void SetIndex(const NativeInfo::Vec2Int index);

public:
	void Add(std::shared_ptr<Object> object);
	void Move(std::shared_ptr<Object> object);
	void Revive(std::shared_ptr<Creature> creature);
	void Remove(const NativeInfo::ObjectInfo& objectInfo);

private:
	void SendSpawnPacketToOldPlayer(std::shared_ptr<Object> object);
	void SendSpawnPacketToNewPlayer(std::shared_ptr<Player> player);

	void SendDespawnPacket(const NativeInfo::ObjectInfo& objectInfo);

public:
	void SendAll(GamePacket::Packet packetType, flatbuffers::Offset<void> packet);
	void SendAllExceptMe(const int64_t& oid, GamePacket::Packet packetType, flatbuffers::Offset<void> packet);

public:
	std::shared_ptr<Player> FindPlayer(const int64_t& oid);
	std::shared_ptr<Projectile> FindProjectile(const int64_t& oid);
	std::shared_ptr<Object> FindObject(const NativeInfo::ObjectInfo& objectInfo);

public:
	void GetCreatures(std::shared_ptr<Creature> creature, const Define::RangeDir& rangeDir, const uint8_t& range, CoreList<std::shared_ptr<Creature>>& creatureList, const bool liveCreatureOnly);

private:
	void GetFrontCreatures(NativeInfo::Vec2Int cellPos, const Define::Dir dir, const uint8_t& range, CoreList<std::shared_ptr<Creature>>& creatureList, const bool liveCreatureOnly);
	void GetCreatures(const NativeInfo::Vec2Int& cellPos, CoreList<std::shared_ptr<Creature>>& creatureList, const bool liveCreatureOnly);

private:
	NativeInfo::Vec2Int index;
	std::map<int64_t, std::shared_ptr<Player>> playerList;
	std::map<int64_t, std::shared_ptr<Projectile>> projectileList;
};