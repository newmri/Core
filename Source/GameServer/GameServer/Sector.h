#pragma once

class Sector : CoreObject
{
	OVERRIDE_OBJECT(Sector)

public:
	void SetIndex(const NativeInfo::Vec2Int index);

public:
	void Add(std::shared_ptr<Creature> creature);
	void Move(std::shared_ptr<Creature> creature);
	void Remove(const Define::ObjectType objectType, const int64_t& oid);

private:
	void SendSpawnPacketToOldPlayer(std::shared_ptr<Creature> creature);
	void SendSpawnPacketToNewPlayer(std::shared_ptr<Player> player);

	void SendDespawnPacket(const Define::ObjectType objectType, const int64_t& oid);

public:
	void SendAll(GamePacket::Packet packetType, flatbuffers::Offset<void> packet);
	void SendAllExceptMe(const int64_t& oid, GamePacket::Packet packetType, flatbuffers::Offset<void> packet);

public:
	std::shared_ptr<Player> FindPlayer(const int64_t& oid);

public:
	void GetObjects(std::shared_ptr<Creature> creature, const Define::RangeDir& rangeDir, const uint8_t& range, CoreList<std::shared_ptr<Creature>>& objectList);

private:
	void GetFrontObjects(NativeInfo::Vec2Int cellPos, const Define::Dir dir, const uint8_t& range, CoreList<std::shared_ptr<Creature>>& objectList);
	void GetObjects(const NativeInfo::Vec2Int& cellPos, CoreList<std::shared_ptr<Creature>>& objectList);

private:
	NativeInfo::Vec2Int index;
	std::map<int64_t, std::shared_ptr<Player>> playerList;
};