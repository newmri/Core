#include "Include.h"

IMPLEMENT_SINGLETON(MapDataManager)

void MapDataManager::Init(void)
{
}

void MapDataManager::Release(void)
{
	GetInstance().~MapDataManager();
}

void MapDataManager::Load(void)
{
	LoadBase();
}

void MapDataManager::LoadBase(void)
{
	char* table = nullptr;
	MAP_LOAD.Load("Data/Map_001.csv", table);
	int32_t minMaxSize = sizeof(NativeInfo::Vec2Int) * 2;

	MapBaseData data;
	memcpy_s(&data, minMaxSize, table, minMaxSize);

	data.count.x = data.max.x - data.min.x + 1;
	data.count.y = data.max.y - data.min.y + 1;

	data.path = new int32_t * [data.count.y];
	data.objects = new int64_t * [data.count.y];
	for (int32_t i = 0; i < data.count.y; ++i)
	{
		data.path[i] = new int32_t[data.count.x]{};
		data.objects[i] = new int64_t[data.count.x]{};
	}

	int32_t offset = minMaxSize;
	for (int32_t y = 0; y < data.count.y; ++y)
	{
		for (int32_t x = 0; x < data.count.x; ++x)
		{
			data.path[y][x] = table[offset];
			offset += SIZE_OF_INT32;
		}
	}

	baseData.push_back(data);
	SAFE_DELETE_ARRAY(table);
}