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
	if (!owner->UseHPMP(this->skillData.HP, this->skillData.MP))
		return false;

	if (!IsValidCoolTime())
		return false;

	return true;
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
