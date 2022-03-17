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

void Creature::CalculateAbility(void)
{
	WRITE_LOCK(this->abilityMutex);
	CHARACTER_DATA_MANAGER.CalculateAbilityByStat(this->creatureInfo);
}