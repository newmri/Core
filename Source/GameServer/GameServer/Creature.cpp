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
	PACKET_SEND_MANAGER.Clear();
	auto objectInfoWithPos = GetObjectInfoWithPos();
	auto packedObjectInfo = Info::ObjectInfoWithPos::Pack(PACKET_SEND_MANAGER.builder, &objectInfoWithPos);
	auto message = GamePacket::CreateSC_REVIVE_RES(PACKET_SEND_MANAGER.builder, packedObjectInfo);
	packetType = GamePacket::Packet_SC_REVIVE_RES;
	packet = message.Union();
}

void Creature::CalculateAbility(void)
{
	WRITE_LOCK(this->abilityMutex);
	CHARACTER_DATA_MANAGER.CalculateAbilityByStat(this->creatureInfo);
}

void Creature::AddSkill(const int32_t skillID)
{
	
}

void Creature::UseSkill(const int32_t skillID)
{
	READ_LOCK(this->skillMutex);

	auto iter = this->skillList.find(skillID);
	if(IS_SAME(iter, this->skillList.end()))
		return;

	iter->second->UseSkill();
}

bool Creature::OnGetDamage(GamePacket::DamageInfoT& damageInfo, const Define::AbilityType defenceType)
{
	damageInfo.damage = CoreUtil::Max(0, damageInfo.damage - GetAbility(defenceType));

	if (IS_SAME(0, damageInfo.damage))
		return false;

	AddHP(-damageInfo.damage);

	damageInfo.object_info = GetObjectInfo();

	return true;
}

void Creature::Revive(void)
{
	if (!IsDead())
		return;

	if (this->deadTime + Define::ReviveTime_COOL_TIME > CORE_TIME_MANAGER.GetNowSeconds())
		return;

	SetState(Define::ObjectState_IDLE);
	ZONE_MANAGER.Revive(Object::downcasted_shared_from_this<Creature>());
}
