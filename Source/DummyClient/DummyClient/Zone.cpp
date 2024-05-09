#include "Include.h"

Zone::Zone()
{

}

Zone::Zone(const int32_t id) : id(id)
{

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
	this->data.objects = new NativeInfo::ObjectInfo * [this->data.size.y];
	for (int32_t i = 0; i < this->data.size.y; ++i)
	{
		this->data.path[i] = new Define::PathType[this->data.size.x]{};
		this->data.objects[i] = new NativeInfo::ObjectInfo[this->data.size.x]{};
	}

	int32_t offset = minMaxSize;
	NativeInfo::Vec2Int index;
	for (; index.y < this->data.size.y; ++index.y)
	{
		for (index.x = 0; index.x < this->data.size.x; ++index.x)
		{
			this->data.path[index.y][index.x] = (Define::PathType)table[offset];

			offset += SIZE_OF_UINT8;
		}
	}

	SAFE_DELETE_ARRAY(table);
}

bool Zone::CanMove(const NativeInfo::Vec2Int& cellDestPos, const bool checkObjects)
{
	if (!IsValidCellPos(cellDestPos))
		return false;

	NativeInfo::Vec2Int index = CellPosToIndex(cellDestPos);

	READ_LOCK(this->mutex);

	return ((IS_SAME(Define::PathType_PATH, this->data.path[index.y][index.x]) ||
		IS_SAME(Define::PathType_START, this->data.path[index.y][index.x])) &&
		(!checkObjects || IS_SAME(Define::ObjectType_NONE, this->data.objects[index.y][index.x].objectType)));
}

void Zone::Move(std::shared_ptr<Object> object, const Info::ObjectInfoWithPosT& objectInfoWithPos)
{
	object->SetObjectInfoWithPos(objectInfoWithPos);

	NativeInfo::Vec2Int index = CellPosToIndex(objectInfoWithPos.pos_info.pos);
	SetObject(objectInfoWithPos.object_info, index);
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

void Zone::SetObject(const NativeInfo::ObjectInfo& objectInfo, const NativeInfo::Vec2Int& index)
{
	WRITE_LOCK(this->mutex);

	this->data.objects[index.y][index.x] = objectInfo;
}
