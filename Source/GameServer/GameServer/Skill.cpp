#include "Include.h"

Skill::Skill()
{

}

Skill::~Skill()
{

}

void Skill::Init(void)
{

}

Skill::Skill(const std::shared_ptr<Creature> owner, const SkillData& skillData)
	: owner(owner), skillData(skillData)
{
	
}

void Skill::UseSkill(void)
{
	if (!this->owner->UseHPMP(this->skillData.HP, this->skillData.MP))
		return;

	if (!IsValidCoolTime())
		return;

	{
		PACKET_SEND_MANAGER.Clear();
		auto packetObjectInfo = this->owner->GetPackedObjectInfo();
		auto message = GamePacket::CreateSC_USE_SKILL_RES(PACKET_SEND_MANAGER.builder, &packetObjectInfo);
		ZONE_MANAGER.SendAllExceptMe(this->owner->GetMapID(), this->owner->GetOID(), GamePacket::Packet_SC_USE_SKILL_RES, message.Union(), this->owner->GetPos());
	}

	CORE_TIME_DELEGATE_MANAGER.Push(
		CoreTimeDelegate<>(std::bind(&Skill::DoAction, shared_from_this()), this->skillData.skillHitTime));
}

void Skill::DoAction(void)
{
	CoreList<std::shared_ptr<Creature>> objectList;
	ZONE_MANAGER.GetCreatures(this->owner, this->skillData.rangeDir, this->skillData.range, objectList, true);

	if (objectList.empty())
		return;

	PACKET_SEND_MANAGER.Clear();

	std::vector<flatbuffers::Offset<GamePacket::DamageInfo>> sendList;

	auto iter_begin = objectList.begin();
	auto iter_end = objectList.end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		DoDamage((*iter_begin), sendList);

		if (CorePacket::MAX_PLAYER_INFO_SEND_COUNT <= sendList.size())
		{
			auto message = GamePacket::CreateSC_GET_DAMAGE_NOTI(PACKET_SEND_MANAGER.builder, PACKET_SEND_MANAGER.builder.CreateVector(sendList));
			ZONE_MANAGER.SendAll(this->owner->GetMapID(), GamePacket::Packet_SC_GET_DAMAGE_NOTI, message.Union(), this->owner->GetPos());
			sendList.clear();
			PACKET_SEND_MANAGER.Clear();
		}
	}

	{
		auto message = GamePacket::CreateSC_GET_DAMAGE_NOTI(PACKET_SEND_MANAGER.builder, PACKET_SEND_MANAGER.builder.CreateVector(sendList));
		ZONE_MANAGER.SendAll(this->owner->GetMapID(), GamePacket::Packet_SC_GET_DAMAGE_NOTI, message.Union(), this->owner->GetPos());
	}
}

void Skill::DoDamage(std::shared_ptr<Object> target, std::vector<flatbuffers::Offset<GamePacket::DamageInfo>>& sendList)
{
	GamePacket::DamageInfoT damageInfo;
	damageInfo.damage = this->owner->GetAbility(this->skillData.damageType) + this->skillData.damage;
	damageInfo.is_critical = CORE_RANDOM_MANAGER_INT.GetRandomByPercent(this->owner->GetAbility(this->skillData.criticalType), SKILL_MAX_RATE);

	if (!target->OnGetDamage(damageInfo, this->skillData.defenceType))
		return;

	sendList.push_back(GamePacket::DamageInfo::Pack(PACKET_SEND_MANAGER.builder, &damageInfo));
}

Define::SkillType Skill::GetSkillType(void)
{
	return this->skillData.skillType;
}

int32_t Skill::GetMapID(void)
{
	return this->owner->GetMapID();
}

NativeInfo::Vec2Int Skill::GetPos(void)
{
	return this->owner->GetPos();
}

bool Skill::IsValidCoolTime(void)
{
	TIME_VALUE nowTime = CORE_TIME_MANAGER.GetNowMilliSeconds();

	if (this->lastSkillUseTime)
	{
		TIME_VALUE timeDiff = nowTime - this->lastSkillUseTime;
		if (timeDiff < this->skillData.coolTime)
			return false;
	}

	this->lastSkillUseTime = nowTime;

	return true;
}
