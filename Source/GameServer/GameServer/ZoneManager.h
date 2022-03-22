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

private:
	CoreVector<std::shared_ptr<Zone>> zoneList;
};