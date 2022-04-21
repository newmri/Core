#pragma once

class Zone : CoreObject
{
	OVERRIDE_OBJECT(Zone)

public:
	Zone(const int32_t id);

private:
	void InitSector(void);

public:
	bool EnterStartPos(std::shared_ptr<Object> object, const bool checkObjects);
	bool Enter(std::shared_ptr<Object> object, const NativeInfo::Vec2Int& cellPos, const bool checkObjects);

private:
	void _Enter(std::shared_ptr<Object> object, Sector* sector, const NativeInfo::Vec2Int& index);

protected:
	bool CanMove(const NativeInfo::Vec2Int& index, const bool checkObjects) const;
	bool IsValidCellPos(const NativeInfo::Vec2Int& cellPos) const;
	NativeInfo::Vec2Int CellPosToIndex(const NativeInfo::Vec2Int& cellPos) const;
	NativeInfo::Vec2Int IndexToCellPos(const NativeInfo::Vec2Int& index) const;
	Sector* GetSector(const NativeInfo::Vec2Int& index);

public:
	bool Move(std::shared_ptr<Object> object, const NativeInfo::Vec2Int& cellDestPos, const bool checkObjects);

public:
	bool Leave(std::shared_ptr<Object> object);

private:
	void _Leave(std::shared_ptr<Object> object, Sector* sector, const NativeInfo::Vec2Int& index);

public:
	void SendAll(GamePacket::Packet packetType, flatbuffers::Offset<void> packet, const NativeInfo::Vec2Int& cellPos);
	void SendAllExceptMe(const int64_t& oid, GamePacket::Packet packetType, flatbuffers::Offset<void> packet, const NativeInfo::Vec2Int& cellPos);

public:
	void GetCreatures(std::shared_ptr<Creature> creature, const Define::RangeDir& rangeDir, const uint8_t& range, CoreList<std::shared_ptr<Creature>>& creatureList, const bool liveCreatureOnly);

public:
	void Revive(std::shared_ptr<Creature> creature);

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