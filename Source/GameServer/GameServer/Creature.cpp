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

int32_t Creature::GetAbility(const Define::AbilityType abilityType)
{
	READ_LOCK(this->abilityMutex);
	return this->creatureInfo.ability.value[abilityType];
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

Define::Dir Creature::GetDir(void)
{
	READ_LOCK(this->infoMutex);
	return this->creatureInfo.pos_info.moveDir;
}

float Creature::GetSpeed(const Define::SpeedType speedType)
{
	READ_LOCK(this->infoMutex);
	return GetSpeedWithNoLock(speedType);
}

bool Creature::IsDead(void)
{
	return (IS_SAME(Define::CreatureState_DEAD, GetState()));
}

Define::CreatureState Creature::GetState(void)
{
	READ_LOCK(this->infoMutex);
	return GetStateWithNoLock();
}

NativeInfo::Vec2Int Creature::GetPosWithNoLock(void) const
{
	return this->creatureInfo.pos_info.pos;
}

float Creature::GetSpeedWithNoLock(const Define::SpeedType speedType) const
{
	return this->creatureInfo.speed.value[speedType];
}

Define::CreatureState Creature::GetStateWithNoLock(void) const
{
	return this->creatureInfo.pos_info.state;
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
		SetStateWithNoLock(Define::CreatureState_DEAD);
	}
}

std::tuple<int32_t, int32_t> Creature::GetHPMP(void)
{
	READ_LOCK(this->infoMutex);
	return std::tuple<int32_t, int32_t>(this->creatureInfo.hp, this->creatureInfo.mp);
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

void Creature::MakeRevivePacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
	auto creatureInfo = GetInfo();
	PACKET_SEND_MANAGER.builder.Clear();
	auto packedPos = flatbuffers::PackPositionInfo(creatureInfo.pos_info);
	auto message = GamePacket::CreateSC_REVIVE_RES(PACKET_SEND_MANAGER.builder, GetObjectType(), GetOID(), &packedPos);
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
	WRITE_LOCK(this->skillMutex);

	SkillData skillData;
	if (!CHARACTER_DATA_MANAGER.GetSkillData(skillID, skillData))
		return;

	this->skillList[skillID] = Skill(this->shared_from_this(), skillData);
}

void Creature::UseSkill(const int32_t skillID)
{
	READ_LOCK(this->skillMutex);

	auto iter = this->skillList.find(skillID);
	if(IS_SAME(iter, this->skillList.end()))
		return;

	iter->second.UseSkill();
}

bool Creature::OnGetDamage(GamePacket::DamageInfoT& damageInfo, const Define::AbilityType defenceType)
{
	damageInfo.damage = CoreUtil::Max(0, damageInfo.damage - GetAbility(defenceType));

	if (IS_SAME(0, damageInfo.damage))
		return false;

	AddHP(-damageInfo.damage);

	damageInfo.oid = GetOID();
	damageInfo.object_type = GetObjectType();
	return true;
}

void Creature::Revive(void)
{
	if (this->deadTime < CORE_TIME_MANAGER.GetNowSeconds() + reviveTime)
		return;

	SetState(Define::CreatureState_IDLE);
	ZONE_MANAGER.Revive(shared_from_this());
}
