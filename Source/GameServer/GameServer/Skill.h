#pragma once

class Creature;

class Skill : public CoreObject
{
	OVERRIDE_OBJECT(Skill)

public:
	Skill(const std::shared_ptr<Creature> owner, const SkillData& skillData);

public:
	bool UseSkill(void);

private:
	bool IsValidCoolTime(void);

protected:
	std::shared_ptr<Creature> owner;
	SkillData skillData;
	TIME_VALUE lastSkillUseTime = 0;
};