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

void Creature::AddHP(const int32_t HP)
{
	WRITE_LOCK(this->infoMutex);
	this->creatureInfo.hp += HP;

	if (0 >= this->creatureInfo.hp)
	{
		this->creatureInfo.hp = 0;
		this->deadTime = CORE_TIME_MANAGER.GetNowSeconds();
		SetStateWithNoLock(Define::ObjectState_DEAD);
	}
}

std::tuple<int32_t, int32_t> Creature::GetHPMP(void)
{
	READ_LOCK(this->infoMutex);
	return std::tuple<int32_t, int32_t>(this->creatureInfo.hp, this->creatureInfo.mp);
}

void Creature::MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{

}

void Creature::MakeRevivePacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
	GAME_PACKET_SEND_MANAGER.Clear();
	auto objectInfoWithPos = GetObjectInfoWithPos();
	auto packedObjectInfo = Info::ObjectInfoWithPos::Pack(GAME_PACKET_SEND_MANAGER.builder, &objectInfoWithPos);
	auto message = GamePacket::CreateSC_REVIVE_RES(GAME_PACKET_SEND_MANAGER.builder, packedObjectInfo);
	packetType = GamePacket::Packet_SC_REVIVE_RES;
	packet = message.Union();
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

void Creature::OnGetDamage(const GamePacket::DamageInfoT& damageInfo)
{
	AddHP(-damageInfo.damage);
}

void Creature::Revive(void)
{
	if (!IsDead())
		return;

	if (this->deadTime + this->reviveTime > CORE_TIME_MANAGER.GetNowSeconds())
		return;

	SetState(Define::ObjectState_IDLE);
}
