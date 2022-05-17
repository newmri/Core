#pragma once

class ProjectileSkill : public Skill
{
	OVERRIDE_OBJECT(ProjectileSkill)

public:
	ProjectileSkill(const std::shared_ptr<Creature> owner, const SkillData& skillData);

public:
	Define::ProjectileType GetType(void);
	float_t GetSpeed(void);

protected:
	virtual void DoAction(void) override;
};