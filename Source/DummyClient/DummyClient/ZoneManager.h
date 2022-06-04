#pragma once

#define ZONE_MANAGER GET_INSTANCE(ZoneManager)

class ZoneManager
{
	DECLARE_SINGLETON(ZoneManager)

public:
	void AddZone(const int32_t id);

public:
	bool CanMove(const int32_t id, const NativeInfo::Vec2Int& cellDestPos, const bool checkObjects = false);
	void Move(std::shared_ptr<Object> object, const Info::ObjectInfoWithPosT& objectInfoWithPos);

private:
	CoreVector<std::shared_ptr<Zone>> zoneList;
};