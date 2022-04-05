#include "Include.h"

Zone::Zone()
{

}

Zone::Zone(const int32_t id) : id(id)
{
	Init();
}

Zone::~Zone()
{
	SAFE_DELETE_2_ARRAY(this->sectors, this->sectorCount.y);
}

void Zone::Init(void)
{
	std::stringstream filePath;
	filePath << "Data/Map_";
	filePath << std::setw(3) << std::setfill('0') << this->id;
	filePath << ".csv";

	char* table = nullptr;
	MAP_LOAD.Load(filePath.str(), table);

	int32_t minMaxSize = sizeof(NativeInfo::Vec2Int) * 2;

	memcpy_s(&this->data, minMaxSize, table, minMaxSize);

	this->data.size.x = this->data.max.x - this->data.min.x + 1;
	this->data.size.y = this->data.max.y - this->data.min.y + 1;

	this->data.path = new Define::PathType * [this->data.size.y];
	this->data.objects = new int64_t * [this->data.size.y];
	for (int32_t i = 0; i < this->data.size.y; ++i)
	{
		this->data.path[i] = new Define::PathType[this->data.size.x]{};
		this->data.objects[i] = new int64_t[this->data.size.x]{};
	}

	int32_t offset = minMaxSize;
	NativeInfo::Vec2Int index;
	for (; index.y < this->data.size.y; ++index.y)
	{
		for (index.x = 0; index.x < this->data.size.x; ++index.x)
		{
			this->data.path[index.y][index.x] = (Define::PathType)table[offset];
			if (IS_SAME(Define::PathType_START, this->data.path[index.y][index.x]))
				this->data.startPos = IndexToCellPos(index);

			offset += SIZE_OF_UINT8;
		}
	}

	SAFE_DELETE_ARRAY(table);
	InitSector();
}

void Zone::InitSector(void)
{
	// 1~40ĭ = 1Sector
	// 41~60ĭ = 2Sector
	this->sectorCount.x = (this->data.size.x + this->SectorCells - 1) / this->SectorCells;
	this->sectorCount.y = (this->data.size.y + this->SectorCells - 1) / this->SectorCells;

	//std::cout << this->sectorCount.x << std::endl;
	//std::cout << this->sectorCount.y << std::endl;

	this->sectors = new Sector*[this->sectorCount.y];
	for (int32_t i = 0; i < this->sectorCount.y; ++i)
		this->sectors[i] = new Sector[this->sectorCount.x];

	NativeInfo::Vec2Int index;
	for (; index.y < this->sectorCount.y; ++index.y)
	{
		for (index.x = 0; this->sectorCount.x < this->sectorCount.x; ++index.x)
		{
			this->sectors[index.y][index.x].SetIndex(index);
		}
	}
}

bool Zone::EnterStartPos(std::shared_ptr<Creature> creature, const bool checkObjects)
{
	auto backupPos = creature->GetPos();
	creature->SetPos(this->data.startPos);

	if (!Enter(creature, this->data.startPos, checkObjects))
	{
		creature->SetPos(backupPos);
		return false;
	}

	return true;
}

bool Zone::Enter(std::shared_ptr<Creature> creature, const NativeInfo::Vec2Int& cellPos, const bool checkObjects)
{
	if (!IsValidCellPos(cellPos))
		return false;

	NativeInfo::Vec2Int index = CellPosToIndex(cellPos);

	Sector* sector = GetSector(index);

	WRITE_LOCK(this->mutex);

	if (!CanMove(index, checkObjects))
		return false;

	_Enter(creature, sector, index);
	return true;
}

void Zone::_Enter(std::shared_ptr<Creature> creature, Sector* sector, const NativeInfo::Vec2Int& index)
{
	this->data.objects[index.y][index.x] = creature->GetOID();
	sector->Add(creature);
}

bool Zone::CanMove(const NativeInfo::Vec2Int& index, const bool checkObjects = false) const
{
	return (IS_SAME(Define::PathType_PATH, this->data.path[index.y][index.x]) || IS_SAME(Define::PathType_START, this->data.path[index.y][index.x]))
		&& (!checkObjects || IS_SAME(0, this->data.objects[index.y][index.x]));
}

bool Zone::IsValidCellPos(const NativeInfo::Vec2Int& cellPos) const
{
	return (IsBetween<int32_t>(cellPos.x, this->data.min.x, this->data.max.x) && IsBetween<int32_t>(cellPos.y, this->data.min.y, this->data.max.y));
}

NativeInfo::Vec2Int Zone::CellPosToIndex(const NativeInfo::Vec2Int& cellPos) const
{
	return NativeInfo::Vec2Int(cellPos.x - this->data.min.x, this->data.max.y - cellPos.y);
}

NativeInfo::Vec2Int Zone::IndexToCellPos(const NativeInfo::Vec2Int& index) const
{
	return NativeInfo::Vec2Int(index.x + this->data.min.x, this->data.max.y - index.y);
}

Sector* Zone::GetSector(const NativeInfo::Vec2Int& index)
{
	return &this->sectors[index.y / this->SectorCells][index.x / this->SectorCells];
}

bool Zone::Move(std::shared_ptr<Creature> creature, const NativeInfo::Vec2Int& cellDestPos, const bool checkObjects)
{
	NativeInfo::Vec2Int cellSourcePos = creature->GetPos();
	if (!IsValidCellPos(cellSourcePos) || !IsValidCellPos(cellDestPos))
		return false;

	if (cellSourcePos == cellDestPos)
		return false;

	NativeInfo::Vec2Int sourceIndex = CellPosToIndex(cellSourcePos);
	NativeInfo::Vec2Int destIndex = CellPosToIndex(cellDestPos);

	Sector* sourceSector = GetSector(sourceIndex);
	Sector* destSector = GetSector(destIndex);

	WRITE_LOCK(this->mutex);

	if (!CanMove(destIndex, checkObjects))
		return false;

	creature->SetMove(Define::CreatureState_WALK, cellDestPos);

	if (sourceSector != destSector)
	{
		_Leave(creature, sourceSector, sourceIndex);
		_Enter(creature, destSector, destIndex);
	}

	else
	{
		destSector->Move(creature);
	}

	return true;
}

bool Zone::Leave(std::shared_ptr<Creature> creature)
{
	if (!IsValidCellPos(creature->GetPos()))
		return false;

	NativeInfo::Vec2Int index = CellPosToIndex(creature->GetPos());

	Sector* sector = GetSector(index);

	WRITE_LOCK(this->mutex);
	_Leave(creature, sector, index);
	return true;
}

void Zone::_Leave(std::shared_ptr<Creature> creature, Sector* sector, const NativeInfo::Vec2Int& index)
{
	this->data.objects[index.y][index.x] = 0;
	sector->Remove(creature->GetObjectType(), creature->GetOID());
}

void Zone::SendAllExceptMe(const int64_t& oid, GamePacket::Packet packetType, flatbuffers::Offset<void> packet, const NativeInfo::Vec2Int& cellPos)
{
	if (!IsValidCellPos(cellPos))
		return;

	GetSector(CellPosToIndex(cellPos))->SendAllExceptMe(oid, packetType, packet);
}

void Zone::GetObjects(std::shared_ptr<Creature> creature, const Define::RangeDir& rangeDir, const uint8_t& range, CoreList<std::shared_ptr<Creature>>& objectList)
{
	if (!IsValidCellPos(creature->GetPos()))
		return;

	NativeInfo::Vec2Int index = CellPosToIndex(creature->GetPos());

	Sector* sector = GetSector(index);

	WRITE_LOCK(this->mutex);
	sector->GetObjects(creature, rangeDir, range, objectList);
}
