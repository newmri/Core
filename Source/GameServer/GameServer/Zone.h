#pragma once

class Zone : CoreObj
{
	OVERRIDE_OBJ(Zone)

public:
	Zone(const int32_t id);

public:
	bool Enter(const Define::ObjectType objType, const int64_t uid, const NativeInfo::Vec2Int& cellPos, const bool checkObjects = false);

private:
	void _Enter(const Define::ObjectType objType, const int64_t uid, const NativeInfo::Vec2Int& index);

protected:
	bool CanMove(const NativeInfo::Vec2Int& index, const bool checkObjects) const;
	bool IsValidCellPos(const NativeInfo::Vec2Int& cellPos) const;
	NativeInfo::Vec2Int CellPosToIndex(const NativeInfo::Vec2Int& cellPos) const;

public:
	bool Move(const Define::ObjectType objType, const int64_t uid,
		const NativeInfo::Vec2Int& cellSourcePos, const NativeInfo::Vec2Int& cellDestPos, const bool checkObjects = false);

public:
	bool Leave(const Define::ObjectType objType, const int64_t uid, const NativeInfo::Vec2Int& cellPos);

private:
	bool _Leave(const Define::ObjectType objType, const int64_t uid, const NativeInfo::Vec2Int& index);

private:
	int32_t id;
	MapData data;

private:
	CACHE_ALIGN std::shared_mutex mutex;
};