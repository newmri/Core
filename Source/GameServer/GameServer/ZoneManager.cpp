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

bool ZoneManager::EnterStartPos(const int32_t id, const Define::ObjectType objectType, const int64_t uid, NativeInfo::Vec2Int& cellPos, const bool checkObjects)
{
	return this->zoneList[id]->EnterStartPos(objectType, uid, cellPos, checkObjects);
}

bool ZoneManager::Enter(const int32_t id, const Define::ObjectType objectType, const int64_t uid, const NativeInfo::Vec2Int& cellPos, const bool checkObjects)
{
	return this->zoneList[id]->Enter(objectType, uid, cellPos, checkObjects);
}

bool ZoneManager::Move(const int32_t id, const Define::ObjectType objectType, const int64_t uid, const NativeInfo::Vec2Int& cellSourcePos, const NativeInfo::Vec2Int& cellDestPos, const bool checkObjects)
{
	return this->zoneList[id]->Move(objectType, uid, cellSourcePos, cellDestPos, checkObjects);
}

bool ZoneManager::Leave(const int32_t id, const Define::ObjectType objectType, const int64_t uid, const NativeInfo::Vec2Int& cellPos)
{
	return this->zoneList[id]->Leave(objectType, uid, cellPos);
}