#pragma once

class ProjectileSkill : public Skill
{
	OVERRIDE_OBJECT(ProjectileSkill)

public:
	ProjectileSkill(const std::shared_ptr<Creature> owner, const SkillData& skillData);

protected:
	virtual void DoAction(void);
};