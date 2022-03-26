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

int32_t Creature::GetMapID(void) const
{
	return this->creatureInfo.pos_info.mapID;
}

NativeInfo::Vec2Int Creature::GetPos(void) const
{
	return this->creatureInfo.pos_info.pos;
}

void Creature::SetDirection(const NativeInfo::Vec2Int& destPos)
{
	NativeInfo::Vec2Int dir = destPos - GetPos();
	std::cout << "speed: " << dir.GetSqrMagnitude() << std::endl;

	this->creatureInfo.pos_info.moveDir = GetPos().GetDirection(destPos);
}

void Creature::SetPos(const NativeInfo::Vec2Int& pos)
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