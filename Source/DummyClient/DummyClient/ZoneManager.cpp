#include "Include.h"

IMPLEMENT_SINGLETON(ZoneManager)

void ZoneManager::Init(void)
{
}

void ZoneManager::Release(void)
{
	GetInstance().~ZoneManager();
}

void ZoneManager::AddZone(const int32_t id)
{
	this->zoneList.push_back(std::make_unique<Zone>(id));
}

bool ZoneManager::CanMove(const int32_t id, const NativeInfo::Vec2Int& cellDestPos, const bool checkObjects)
{
	return this->zoneList[id]->CanMove(cellDestPos, checkObjects);
}

void ZoneManager::Move(std::shared_ptr<Object> object, const Info::ObjectInfoWithPosT& objectInfoWithPos)
{
	this->zoneList[object->GetMapID()]->Move(object, objectInfoWithPos);
}
