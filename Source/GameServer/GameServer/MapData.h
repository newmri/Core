#pragma once

struct MapData
{
	MapData() CORE_DEFAULT;
	~MapData()
	{
		SAFE_DELETE_2_ARRAY(path, size.y);
		SAFE_DELETE_2_ARRAY(objects, size.y);
	}

	NativeInfo::Vec2Int min;
	NativeInfo::Vec2Int max;
	NativeInfo::Vec2Int size;
	Define::PathType** path = nullptr;
	int64_t** objects = nullptr;

	NativeInfo::Vec2Int startIndex;
	NativeInfo::Vec2Int startCellPos;
};