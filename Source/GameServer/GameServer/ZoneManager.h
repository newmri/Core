#pragma once

#define ZONE_MANAGER GET_INSTANCE(ZoneManager)

class ZoneManager
{
	DECLARE_SINGLETON(ZoneManager)

public:
	void AddZone(const int32_t id);

public:
	bool EnterStartPos(const int32_t id, std::shared_ptr<Creature> creature, const bool checkObjects = false);
	bool Enter(const int32_t id, std::shared_ptr<Creature> creature, const bool checkObjects = false);

public:
	bool Move(std::shared_ptr<Creature> creature, const NativeInfo::Vec2Int& cellDestPos, const bool checkObjects = false);

public:
	bool Leave(std::shared_ptr<Creature> creature);

public:
	void SendAll(const int32_t id, GamePacket::Packet packetType, flatbuffers::Offset<void> packet, const NativeInfo::Vec2Int& cellPos);
	void SendAllExceptMe(const int32_t id, const int64_t& oid, GamePacket::Packet packetType, flatbuffers::Offset<void> packet, const NativeInfo::Vec2Int& cellPos);

public:
	void GetCreatures(std::shared_ptr<Creature> creature, const Define::RangeDir& rangeDir, const uint8_t& range, CoreList<std::shared_ptr<Creature>>& objectList, const bool liveCreatureOnly = false);

public:
	void Revive(std::shared_ptr<Creature> creature);

private:
	CoreVector<std::shared_ptr<Zone>> zoneList;
};