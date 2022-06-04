#pragma once

class Zone : CoreObject
{
	OVERRIDE_OBJECT(Zone)

public:
	Zone(const int32_t id);

public:
	bool CanMove(const NativeInfo::Vec2Int& cellDestPos, const bool checkObjects);
	void Move(std::shared_ptr<Object> object, const Info::ObjectInfoWithPosT& objectInfoWithPos);

protected:
	bool IsValidCellPos(const NativeInfo::Vec2Int& cellPos) const;
	NativeInfo::Vec2Int CellPosToIndex(const NativeInfo::Vec2Int& cellPos) const;
	NativeInfo::Vec2Int IndexToCellPos(const NativeInfo::Vec2Int& index) const;

private:
	void SetObject(const NativeInfo::ObjectInfo& objectInfo, const NativeInfo::Vec2Int& index);

private:
	int32_t id = 0;
	MapData data;

private:
	CACHE_ALIGN std::shared_mutex mutex;
};