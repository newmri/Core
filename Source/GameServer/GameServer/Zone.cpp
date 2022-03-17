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
}

bool Zone::EnterStartPos(const Define::ObjectType objType, const int64_t uid, NativeInfo::Vec2Int& cellPos, const bool checkObjects)
{
	if (!Enter(objType, uid, this->data.startPos, checkObjects))
		return false;

	cellPos = this->data.startPos;
	return true;
}

bool Zone::Enter(const Define::ObjectType objType, const int64_t uid, const NativeInfo::Vec2Int& cellPos, const bool checkObjects)
{
	if (!IsValidCellPos(cellPos))
		return false;

	NativeInfo::Vec2Int index = CellPosToIndex(cellPos);

	WRITE_LOCK(this->mutex);

	if (!CanMove(index, checkObjects))
		return false;

	_Enter(objType, uid, index);
	return true;
}

void Zone::_Enter(const Define::ObjectType objType, const int64_t uid, const NativeInfo::Vec2Int& index)
{
	this->data.objects[index.y][index.x] = uid;
	// 같은 Sector에 있게된 유저들한테 브로드캐스트 필요
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

bool Zone::Move(const Define::ObjectType objType, const int64_t uid,
	const NativeInfo::Vec2Int& cellSourcePos, const NativeInfo::Vec2Int& cellDestPos, const bool checkObjects)
{
	if (!IsValidCellPos(cellSourcePos) || !IsValidCellPos(cellDestPos))
		return false;

	NativeInfo::Vec2Int sourceIndex = CellPosToIndex(cellSourcePos);
	NativeInfo::Vec2Int destIndex = CellPosToIndex(cellDestPos);

	WRITE_LOCK(this->mutex);

	if (!CanMove(destIndex, checkObjects))
		return false;

	_Leave(objType, uid, sourceIndex);
	_Enter(objType, uid, destIndex);
	return true;
}

bool Zone::Leave(const Define::ObjectType objType, const int64_t uid, const NativeInfo::Vec2Int& cellPos)
{
	if (!IsValidCellPos(cellPos))
		return false;

	NativeInfo::Vec2Int index = CellPosToIndex(cellPos);

	WRITE_LOCK(this->mutex);

	_Leave(objType, uid, index);
	return true;
}

void Zone::_Leave(const Define::ObjectType objType, const int64_t uid, const NativeInfo::Vec2Int& index)
{
	this->data.objects[index.y][index.x] = 0;
	// 같은 Sector에 있던 유저들한테 브로드캐스트 필요
}