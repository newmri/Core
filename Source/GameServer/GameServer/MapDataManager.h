#pragma once

#define MAP_DATA_MANAGER GET_INSTANCE(MapDataManager)

class MapDataManager
{
	DECLARE_SINGLETON(MapDataManager)

public:
	void Load(void);

private:
	void LoadBase(void);

private:
	CoreVector<MapBaseData> baseData;
};