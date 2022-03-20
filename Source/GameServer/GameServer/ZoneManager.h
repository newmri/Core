#pragma once

#define ZONE_MANAGER GET_INSTANCE(ZoneManager)

class ZoneManager
{
	DECLARE_SINGLETON(ZoneManager)

public:
	void AddZone(const int32_t id);

public:
	bool EnterStartPos(const int32_t id, const Define::ObjectType objectType, const int64_t uid, NativeInfo::Vec2Int& cellPos, const bool checkObjects = false);
	bool Enter(const int32_t id, const Define::ObjectType objectType, const int64_t uid, const NativeInfo::Vec2Int& cellPos, const bool checkObjects = false);

public:
	bool Move(const int32_t id, const Define::ObjectType objectType, const int64_t uid,
		const NativeInfo::Vec2Int& cellSourcePos, const NativeInfo::Vec2Int& cellDestPos, const bool checkObjects = false);

public:
	bool Leave(const int32_t id, const Define::ObjectType objectType, const int64_t uid, const NativeInfo::Vec2Int& cellPos);

private:
	CoreVector<std::shared_ptr<Zone>> zoneList;
};