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

bool Skill::UseSkill(void)
{
	if (!this->owner->UseHPMP(this->skillData.HP, this->skillData.MP))
		return false;

	if (!IsValidCoolTime())
		return false;

	return true;
}

void Skill::DoAction(void)
{

}

void Skill::DoDamage(std::shared_ptr<Object> target, std::vector<flatbuffers::Offset<GamePacket::DamageInfo>>& sendList)
{

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
