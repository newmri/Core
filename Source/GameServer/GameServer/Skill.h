#pragma once

class Creature;

static const int32_t SKILL_MAX_RATE = 1000;

class Skill : public CoreObject
{
	OVERRIDE_OBJECT(Skill)

public:
	Skill(const std::shared_ptr<Creature> owner, const SkillData& skillData);

public:
	void UseSkill(void);

public:
	void DoDamage(void);

private:
	bool IsValidCoolTime(void);

protected:
	std::shared_ptr<Creature> owner;
	SkillData skillData;
	TIME_VALUE lastSkillUseTime = 0;
};