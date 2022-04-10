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

bool ZoneManager::EnterStartPos(const int32_t id, std::shared_ptr<Creature> creature, const bool checkObjects)
{
	return this->zoneList[id]->EnterStartPos(creature, checkObjects);
}

bool ZoneManager::Enter(const int32_t id, std::shared_ptr<Creature> creature, const bool checkObjects)
{
	return this->zoneList[id]->Enter(creature, creature->GetPos(), checkObjects);
}

bool ZoneManager::Move(std::shared_ptr<Creature> creature, const NativeInfo::Vec2Int& cellDestPos, const bool checkObjects)
{
	return this->zoneList[creature->GetMapID()]->Move(creature, cellDestPos, checkObjects);
}

bool ZoneManager::Leave(std::shared_ptr<Creature> creature)
{
	return this->zoneList[creature->GetMapID()]->Leave(creature);
}

void ZoneManager::SendAll(const int32_t id, GamePacket::Packet packetType, flatbuffers::Offset<void> packet, const NativeInfo::Vec2Int& cellPos)
{
	this->zoneList[id]->SendAll(packetType, packet, cellPos);
}

void ZoneManager::SendAllExceptMe(const int32_t id, const int64_t& oid, GamePacket::Packet packetType, flatbuffers::Offset<void> packet, const NativeInfo::Vec2Int& cellPos)
{
	this->zoneList[id]->SendAllExceptMe(oid, packetType, packet, cellPos);
}

void ZoneManager::GetCreatures(std::shared_ptr<Creature> creature, const Define::RangeDir& rangeDir, const uint8_t& range, CoreList<std::shared_ptr<Creature>>& objectList, const bool liveCreatureOnly)
{
	this->zoneList[creature->GetMapID()]->GetCreatures(creature, rangeDir, range, objectList, liveCreatureOnly);
}
