#include "Include.h"

Skill::Skill()
{
	Init();
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
	Init();
}

void Skill::UseSkill(void)
{
	if (!owner->UseHPMP(this->skillData.HP, this->skillData.MP))
		return;

	if (!IsValidCoolTime())
		return;

	PACKET_SEND_MANAGER.builder.Clear();
	auto message = GamePacket::CreateSC_USE_SKILL_RES(PACKET_SEND_MANAGER.builder, owner->GetObjectType(), owner->GetOID());
	ZONE_MANAGER.SendAllExceptMe(owner->GetMapID(), owner->GetOID(), GamePacket::Packet_SC_USE_SKILL_RES, message.Union(), owner->GetPos());

	CoreList<std::shared_ptr<Creature>> objectList;
	ZONE_MANAGER.GetObjects(owner, this->skillData.rangeDir, this->skillData.range, objectList);

	if (objectList.empty())
		return;

	int32_t damage = 0;
	bool isCritical = false;

	auto iter_begin = objectList.begin();
	auto iter_end = objectList.end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		damage = owner->GetAbility(this->skillData.damageType) + this->skillData.damage;
		isCritical = CORE_RANDOM_MANAGER_INT.GetRandomByPercent(owner->GetAbility(this->skillData.criticalType), SKILL_MAX_RATE);

		(*iter_begin)->OnGetDamage(damage, this->skillData.defenceType, isCritical);
	}

	// SendAllExceptMe ÇÊ¿ä
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
