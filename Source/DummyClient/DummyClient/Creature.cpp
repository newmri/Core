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

Info::CreatureInfoT Creature::GetCreatureInfo(void)
{
	READ_LOCK(this->infoMutex);
	return this->creatureInfo;
}

int32_t Creature::GetAbility(const Define::AbilityType abilityType)
{
	READ_LOCK(this->abilityMutex);
	return this->creatureInfo.ability.value[abilityType];
}

float Creature::GetSpeed(const Define::SpeedType speedType)
{
	READ_LOCK(this->infoMutex);
	return GetSpeedWithNoLock(speedType);
}

float Creature::GetSpeedWithNoLock(const Define::SpeedType speedType) const
{
	return this->creatureInfo.speed.value[speedType];
}

bool Creature::UseHPMP(const int32_t HP, const int32_t MP)
{
	WRITE_LOCK(this->infoMutex);

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
	WRITE_LOCK(this->infoMutex);
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
	READ_LOCK(this->infoMutex);
	return std::tuple<int32_t, int32_t>(this->creatureInfo.hp, this->creatureInfo.mp);
}

void Creature::MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{

}

void Creature::AddSkill(const int32_t skillID)
{

}

void Creature::UseSkill(const int32_t skillID)
{
	READ_LOCK(this->skillMutex);

	auto iter = this->skillList.find(skillID);
	if (IS_SAME(iter, this->skillList.end()))
		return;

	iter->second->UseSkill();
}

bool Creature::OnGetDamage(const GamePacket::DamageInfoT& damageInfo)
{
	if (IsDead())
		return false;

	return AddHP(-damageInfo.damage);
}

void Creature::Revive(void)
{
	if (!IsDead())
		return;

	SetState(Define::ObjectState_IDLE);
}
