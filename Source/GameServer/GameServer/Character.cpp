#include "Include.h"

Character::Character(const int64_t& accountUID, const GamePacket::MyCharacterInfoT& info) :
	CoreCharacter(accountUID, info.uid), info(info)
{
	Init();
}

Character::~Character()
{

}

void Character::Init(void)
{

}

void Character::CalculateAbility(void)
{
	WRITE_LOCK(this->abilityMutex);
	DATA_MANAGER.CalculateAbilityByStat(this->info);
}