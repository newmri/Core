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
	auto zone = std::make_unique<Zone>(id);
	zone->Init();
	this->zoneList.push_back(std::move(zone));
}

bool ZoneManager::EnterStartPos(const int32_t id, std::shared_ptr<Object> object, const bool checkPath, const bool checkObjects)
{
	return this->zoneList[id]->EnterStartPos(object, checkPath, checkObjects);
}

bool ZoneManager::Enter(const int32_t id, std::shared_ptr<Object> object, const bool checkPath, const bool checkObjects)
{
	return this->zoneList[id]->Enter(object, object->GetPos(), checkPath, checkObjects);
}

std::tuple<bool, std::shared_ptr<Object>> ZoneManager::Move(std::shared_ptr<Object> object, const NativeInfo::Vec2Int& cellDestPos, const bool isRun, const bool checkPath, const bool checkObjects)
{
	return this->zoneList[object->GetMapID()]->Move(object, cellDestPos, isRun, checkPath, checkObjects);
}

bool ZoneManager::Leave(std::shared_ptr<Object> object)
{
	return this->zoneList[object->GetMapID()]->Leave(object);
}

void ZoneManager::SendAll(const int32_t id, GamePacket::Packet packetType, flatbuffers::Offset<void> packet, const NativeInfo::Vec2Int& cellPos)
{
	this->zoneList[id]->SendAll(packetType, packet, cellPos);
}

void ZoneManager::SendAllExceptMe(const int32_t id, const int64_t& oid, GamePacket::Packet packetType, flatbuffers::Offset<void> packet, const NativeInfo::Vec2Int& cellPos)
{
	this->zoneList[id]->SendAllExceptMe(oid, packetType, packet, cellPos);
}

void ZoneManager::GetCreatures(std::shared_ptr<Creature> creature, const Define::RangeDir& rangeDir, const uint8_t& range, CoreList<std::shared_ptr<Creature>>& creatureList, const bool liveCreatureOnly)
{
	this->zoneList[creature->GetMapID()]->GetCreatures(creature, rangeDir, range, creatureList, liveCreatureOnly);
}

void ZoneManager::Revive(std::shared_ptr<Creature> creature)
{
	this->zoneList[creature->GetMapID()]->Revive(creature);
}
