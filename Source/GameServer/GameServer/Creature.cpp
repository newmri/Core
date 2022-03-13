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

void Creature::CalculateAbility(void)
{
	WRITE_LOCK(this->abilityMutex);
	DATA_MANAGER.CalculateAbilityByStat(this->creatureInfo);
}