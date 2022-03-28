#include "Include.h"

Creature::Creature(const Info::CreatureInfoT& creatureInfo) : creatureInfo(creatureInfo)
{
	Init();
}

Creature::~Creature()
{

}

void Creature::Init(void)
{
	
}

void Creature::Update(void)
{

}

Info::CreatureInfoT Creature::GetInfo(void)
{
	READ_LOCK(this->infoMutex);
	return this->creatureInfo;
}

Define::ObjectType Creature::GetObjectType(void) const
{
	return this->creatureInfo.object_type;
}

int64_t Creature::GetOID(void) const
{
	return this->creatureInfo.oid;
}

int32_t Creature::GetMapID(void)
{
	READ_LOCK(this->infoMutex);
	return this->creatureInfo.pos_info.mapID;
}

NativeInfo::Vec2Int Creature::GetPos(void)
{
	READ_LOCK(this->infoMutex);
	return GetPosWithNoLock();
}

NativeInfo::Vec2Int Creature::GetPosWithNoLock(void) const
{
	return this->creatureInfo.pos_info.pos;
}

void Creature::SetMove(const Define::CreatureState state, const NativeInfo::Vec2Int& destPos)
{
	WRITE_LOCK(this->infoMutex);
	SetStateWithNoLock(state);
	SetDirectionWithNoLock(destPos);
	SetPosWithNoLock(destPos);
}

void Creature::SetState(const Define::CreatureState state)
{
	WRITE_LOCK(this->infoMutex);
	SetStateWithNoLock(state);
}

void Creature::SetDirection(const NativeInfo::Vec2Int& destPos)
{
	WRITE_LOCK(this->infoMutex);
	SetDirectionWithNoLock(destPos);
}

void Creature::SetPos(const NativeInfo::Vec2Int& pos)
{
	WRITE_LOCK(this->infoMutex);
	SetPosWithNoLock(pos);
}

void Creature::SetStateWithNoLock(const Define::CreatureState state)
{
	this->creatureInfo.pos_info.state = state;
}

void Creature::SetDirectionWithNoLock(const NativeInfo::Vec2Int& destPos)
{
	NativeInfo::Vec2Int dir = destPos - GetPosWithNoLock();
	std::cout << "speed: " << dir.GetSqrMagnitude() << std::endl;

	this->creatureInfo.pos_info.moveDir = GetPosWithNoLock().GetDirection(destPos);
}

void Creature::SetPosWithNoLock(const NativeInfo::Vec2Int& pos)
{
	this->creatureInfo.pos_info.pos = pos;
}

void Creature::MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
	
}

void Creature::MakeMovePacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
	auto creatureInfo = GetInfo();
	PACKET_SEND_MANAGER.builder.Clear();
	auto packedPos = flatbuffers::PackPositionInfo(creatureInfo.pos_info);
	auto message = GamePacket::CreateSC_MOVE_RES(PACKET_SEND_MANAGER.builder, GetObjectType(), GetOID(), &packedPos);
	packetType = GamePacket::Packet_SC_MOVE_RES;
	packet = message.Union();
}

void Creature::CalculateAbility(void)
{
	WRITE_LOCK(this->abilityMutex);
	CHARACTER_DATA_MANAGER.CalculateAbilityByStat(this->creatureInfo);
}