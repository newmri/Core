#pragma once

class Zone : CoreObj
{
	OVERRIDE_OBJ(Zone)

public:
	Zone(const int32_t id);

public:
	bool CanMove(const NativeInfo::Vec2Int& cellPos, const bool checkObjects = true) const;
	bool IsValidCellPos(const NativeInfo::Vec2Int& cellPos) const;
	NativeInfo::Vec2Int CellPosToIndex(const NativeInfo::Vec2Int& cellPos) const;

private:
	int32_t id;
	MapData data;
};