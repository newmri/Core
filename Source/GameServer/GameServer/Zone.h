#pragma once

class Zone : CoreObject
{
	OVERRIDE_OBJECT(Zone)

public:
	Zone(const int32_t id);

private:
	void InitSector(void);

public:
	bool EnterStartPos(const Define::ObjectType objectType, const int64_t uid, NativeInfo::Vec2Int& cellPos, const bool checkObjects);
	bool Enter(const Define::ObjectType objectType, const int64_t uid, const NativeInfo::Vec2Int& cellPos, const bool checkObjects);

private:
	void _Enter(const Define::ObjectType objectType, const int64_t uid, const NativeInfo::Vec2Int& index);

protected:
	bool CanMove(const NativeInfo::Vec2Int& index, const bool checkObjects) const;
	bool IsValidCellPos(const NativeInfo::Vec2Int& cellPos) const;
	NativeInfo::Vec2Int CellPosToIndex(const NativeInfo::Vec2Int& cellPos) const;
	NativeInfo::Vec2Int IndexToCellPos(const NativeInfo::Vec2Int& index) const;
	Sector* GetSector(const NativeInfo::Vec2Int& index);

public:
	bool Move(const Define::ObjectType objectType, const int64_t uid,
		const NativeInfo::Vec2Int& cellSourcePos, const NativeInfo::Vec2Int& cellDestPos, const bool checkObjects);

public:
	bool Leave(const Define::ObjectType objectType, const int64_t uid, const NativeInfo::Vec2Int& cellPos);

private:
	void _Leave(const Define::ObjectType objectType, const int64_t uid, const NativeInfo::Vec2Int& index);

private:
	int32_t id;
	MapData data;

private:
	int32_t SectorCells = 40;
	Sector** sectors = nullptr;
	NativeInfo::Vec2Int sectorCount;

private:
	CACHE_ALIGN std::shared_mutex mutex;
};