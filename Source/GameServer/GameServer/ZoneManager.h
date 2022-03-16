#pragma once

#define ZONE_MANAGER GET_INSTANCE(ZoneManager)

class ZoneManager
{
	DECLARE_SINGLETON(ZoneManager)

public:
	void AddZone(const int32_t id);

private:
	CoreVector<Zone> zoneList;
};