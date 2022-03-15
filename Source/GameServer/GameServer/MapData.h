#pragma once

struct MapBaseData
{
	MapBaseData() CORE_DEFAULT;
	~MapBaseData()
	{
		SAFE_DELETE_2_ARRAY(path, count.y);
		SAFE_DELETE_2_ARRAY(objects, count.y);
	}

	NativeInfo::Vec2Int min;
	NativeInfo::Vec2Int max;
	NativeInfo::Vec2Int count;
	int32_t** path = nullptr;
	int64_t** objects = nullptr;
};