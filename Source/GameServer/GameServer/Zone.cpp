#include "Include.h"

Zone::Zone()
{

}

Zone::Zone(const int32_t id) : id(id)
{

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
	this->data.objects = new Objects * [this->data.size.y];
	for (int32_t i = 0; i < this->data.size.y; ++i)
	{
		this->data.path[i] = new Define::PathType[this->data.size.x]{};
		this->data.objects[i] = new Objects[this->data.size.x]{};
	}

	int32_t offset = minMaxSize;
	NativeInfo::Vec2Int index;
	for (; index.y < this->data.size.y; ++index.y)
	{
		for (index.x = 0; index.x < this->data.size.x; ++index.x)
		{
			this->data.path[index.y][index.x] = (Define::PathType)table[offset];
			if (IS_SAME(Define::PathType_START, this->data.path[index.y][index.x]))
			{
				this->data.startIndex = index;
				this->data.startCellPos = IndexToCellPos(index);
			}

			offset += SIZE_OF_UINT8;
		}
	}

	SAFE_DELETE_ARRAY(table);
	InitSector();
}

void Zone::InitSector(void)
{
	// 1~40 = 1Sector
	// 41~60 = 2Sector
	this->sectorCount.x = (this->data.size.x + this->SectorCells - 1) / this->SectorCells;
	this->sectorCount.y = (this->data.size.y + this->SectorCells - 1) / this->SectorCells;

	this->sectors = new Sector*[this->sectorCount.y];
	for (int32_t i = 0; i < this->sectorCount.y; ++i)
	{
		this->sectors[i] = new Sector[this->sectorCount.x];
		this->sectors[i]->Init();
	}

	NativeInfo::Vec2Int index;
	for (; index.y < this->sectorCount.y; ++index.y)
	{
		for (index.x = 0; this->sectorCount.x < this->sectorCount.x; ++index.x)
		{
			this->sectors[index.y][index.x].SetIndex(index);
		}
	}
}

bool Zone::EnterStartPos(std::shared_ptr<Object> object, const bool checkPath, const bool checkObjects)
{
	auto backupPos = object->GetPos();
	object->SetPos(this->data.startCellPos);

	if (!Enter(object, this->data.startCellPos, checkPath, checkObjects))
	{
		object->SetPos(backupPos);
		return false;
	}

	return true;
}

bool Zone::Enter(std::shared_ptr<Object> object, const NativeInfo::Vec2Int& cellPos, const bool checkPath, const bool checkObjects)
{
	if (!IsValidCellPos(cellPos))
		return false;

	NativeInfo::Vec2Int index = CellPosToIndex(cellPos);

	auto sector = GetSector(index);

	WRITE_LOCK(this->mutex);

	if (IS_NOT_NULL(CanMove(sector, index, checkPath, checkObjects)))
		return false;

	_Enter(object, sector, index);
	return true;
}

void Zone::_Enter(std::shared_ptr<Object> object, Sector* sector, const NativeInfo::Vec2Int& index)
{
	AddObjectInfo(index, NativeInfo::ObjectInfo(object->GetObjectType(), object->GetOID()));
	sector->Add(object);
}

void Zone::AddObjectInfo(const NativeInfo::Vec2Int& index, const NativeInfo::ObjectInfo& objectInfo)
{
	this->data.objects[index.y][index.x].objectInfo.push_back(objectInfo);
}

void Zone::RemoveObjectInfo(const NativeInfo::Vec2Int& index, const int64_t oid)
{
	this->data.objects[index.y][index.x].objectInfo.erase(
		std::remove_if(this->data.objects[index.y][index.x].objectInfo.begin(),
			this->data.objects[index.y][index.x].objectInfo.end(),
			[&](NativeInfo::ObjectInfo info)
			{
				return IS_SAME(oid, info.oid);
			}), this->data.objects[index.y][index.x].objectInfo.end());
}

std::shared_ptr<Object> Zone::CanMove(Sector* sector, const NativeInfo::Vec2Int& index, const bool checkPath, const bool checkObjects) const
{
	if ((!checkPath || (IS_SAME(Define::PathType_PATH, this->data.path[index.y][index.x]) ||
		IS_SAME(Define::PathType_START, this->data.path[index.y][index.x]))) &&
		checkObjects && !this->data.objects[index.y][index.x].objectInfo.empty())
	{
		auto iter_begin = this->data.objects[index.y][index.x].objectInfo.begin();
		auto iter_end = this->data.objects[index.y][index.x].objectInfo.end();
		for (; iter_begin != iter_end; ++iter_begin)
		{
			if (auto object = sector->FindObject((*iter_begin)); IS_NOT_NULL(object))
			{
				if (object->IsCreature() && !object->IsDead())
					return object;
			}
		}
	}

	return nullptr;
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

std::tuple<bool, std::shared_ptr<Object>> Zone::Move(std::shared_ptr<Object> object, const NativeInfo::Vec2Int& cellDestPos, const bool isRun, const bool checkPath, const bool checkObjects)
{
	NativeInfo::Vec2Int cellSourcePos = object->GetPos();
	if (!IsValidCellPos(cellSourcePos) || !IsValidCellPos(cellDestPos))
		return std::make_tuple(false, nullptr);

	if (cellSourcePos == cellDestPos)
		return std::make_tuple(false, nullptr);

	NativeInfo::Vec2Int sourceIndex = CellPosToIndex(cellSourcePos);
	NativeInfo::Vec2Int destIndex = CellPosToIndex(cellDestPos);

	auto sourceSector = GetSector(sourceIndex);
	auto destSector = GetSector(destIndex);

	WRITE_LOCK(this->mutex);

	if (std::shared_ptr<Object> object = CanMove(destSector, destIndex, checkPath, checkObjects); IS_NOT_NULL(object))
	{
		return std::make_tuple(false, object);
	}

	if(isRun)
		object->SetMove(Define::ObjectState_RUN, cellDestPos);
	else
		object->SetMove(Define::ObjectState_WALK, cellDestPos);

	if (sourceSector != destSector)
	{
		_Leave(object, sourceSector, sourceIndex);
		_Enter(object, destSector, destIndex);
	}

	else
	{
		RemoveObjectInfo(sourceIndex, object->GetOID());
		AddObjectInfo(destIndex, object->GetObjectInfo());

		destSector->Move(object);
	}

	return std::make_tuple(true, nullptr);
}

bool Zone::Leave(std::shared_ptr<Object> object)
{
	if (!IsValidCellPos(object->GetPos()))
		return false;

	NativeInfo::Vec2Int index = CellPosToIndex(object->GetPos());

	auto sector = GetSector(index);

	WRITE_LOCK(this->mutex);
	_Leave(object, sector, index);
	return true;
}

void Zone::_Leave(std::shared_ptr<Object> object, Sector* sector, const NativeInfo::Vec2Int& index)
{
	RemoveObjectInfo(index, object->GetOID());
	sector->Remove(object->GetObjectInfo());
}

void Zone::SendAll(GamePacket::Packet packetType, flatbuffers::Offset<void> packet, const NativeInfo::Vec2Int& cellPos)
{
	if (!IsValidCellPos(cellPos))
		return;

	GetSector(CellPosToIndex(cellPos))->SendAll(packetType, packet);
}

void Zone::SendAllExceptMe(const int64_t& oid, GamePacket::Packet packetType, flatbuffers::Offset<void> packet, const NativeInfo::Vec2Int& cellPos)
{
	if (!IsValidCellPos(cellPos))
		return;

	GetSector(CellPosToIndex(cellPos))->SendAllExceptMe(oid, packetType, packet);
}

void Zone::GetCreatures(std::shared_ptr<Creature> creature, const Define::RangeDir& rangeDir, const uint8_t& range, CoreList<std::shared_ptr<Creature>>& creatureList, const bool liveCreatureOnly)
{
	if (!IsValidCellPos(creature->GetPos()))
		return;

	NativeInfo::Vec2Int index = CellPosToIndex(creature->GetPos());

	auto sector = GetSector(index);

	WRITE_LOCK(this->mutex);
	sector->GetCreatures(creature, rangeDir, range, creatureList, liveCreatureOnly);
}

void Zone::Revive(std::shared_ptr<Creature> creature)
{
	if (!IsValidCellPos(creature->GetPos()))
		return;

	NativeInfo::Vec2Int deadIndex = CellPosToIndex(creature->GetPos());

	auto deadSector = GetSector(deadIndex);
	auto reviveSector = GetSector(this->data.startIndex);

	creature->SetPos(this->data.startCellPos);

	// 사망 섹터와 부활 섹터가 같음
	if (IS_SAME(deadSector, reviveSector))
	{
		{
			WRITE_LOCK(this->mutex);
			RemoveObjectInfo(deadIndex, creature->GetOID());
			AddObjectInfo(this->data.startIndex, NativeInfo::ObjectInfo(creature->GetObjectType(), creature->GetOID()));
		}

		reviveSector->Revive(creature);
	}
	else
	{
		WRITE_LOCK(this->mutex);
		_Leave(creature, deadSector, deadIndex);
		_Enter(creature, reviveSector, this->data.startIndex);
	}
}

