#include "Include.h"

Creature::Creature(const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo) : Object(objectInfoWithPos), creatureInfo(creatureInfo)
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

void Creature::Clear(void)
{
	Object::Clear();

	WRITE_LOCK(this->skillMutex);
	this->skillList.clear();
}

Info::CreatureInfoT Creature::GetCreatureInfo(void)
{
	return this->creatureInfo;
}

int32_t Creature::GetAbility(const Define::AbilityType abilityType)
{
	return this->creatureInfo.ability.value[abilityType];
}

float Creature::GetSpeed(const Define::SpeedType speedType)
{
	return GetSpeedWithNoLock(speedType);
}

float Creature::GetSpeedWithNoLock(const Define::SpeedType speedType) const
{
	return this->creatureInfo.speed.value[speedType];
}

bool Creature::UseHPMP(const int32_t HP, const int32_t MP)
{
	if (IS_SAME(0, HP) && IS_SAME(0, MP))
		return true;

	if (this->creatureInfo.hp < HP || this->creatureInfo.mp < MP)
		return false;

	this->creatureInfo.hp -= HP;
	this->creatureInfo.mp -= MP;
	return true;
}

bool Creature::AddHP(const int32_t HP)
{
	this->creatureInfo.hp += HP;

	if (0 >= this->creatureInfo.hp)
	{
		this->creatureInfo.hp = 0;
		SetStateWithNoLock(Define::ObjectState_DEAD);
		return true;
	}

	return false;
}

std::tuple<int32_t, int32_t> Creature::GetHPMP(void)
{
	return std::tuple<int32_t, int32_t>(this->creatureInfo.hp, this->creatureInfo.mp);
}

bool Creature::OnGetDamage(const GamePacket::DamageInfoT& damageInfo)
{
	if (IsDead())
		return false;

	return AddHP(-damageInfo.damage);
}

bool Creature::Revive(const Info::ObjectInfoWithPosT& objectInfoWithPos)
{
	if (!IsDead())
		return false;

	this->objectInfoWithPos = objectInfoWithPos;
	return true;
}
