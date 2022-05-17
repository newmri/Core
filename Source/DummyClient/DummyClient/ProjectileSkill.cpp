#include "Include.h"

ProjectileSkill::ProjectileSkill()
{
	Init();
}

ProjectileSkill::~ProjectileSkill()
{
}

void ProjectileSkill::Init(void)
{
}

ProjectileSkill::ProjectileSkill(const std::shared_ptr<Creature> owner, const SkillData& skillData) : Skill(owner, skillData)
{
}

Define::ProjectileType ProjectileSkill::GetType(void)
{
	return this->skillData.projectileType;
}

float_t ProjectileSkill::GetSpeed(void)
{
	return this->skillData.projectileSpeed;
}

void ProjectileSkill::DoAction(void)
{
	Info::ObjectInfoT objectInfo;
	objectInfo.pos_info = this->owner->GetPosInfo();
	objectInfo.pos_info.pos = objectInfo.pos_info.pos.GetFrontPos(this->owner->GetDir());

	OBJECT_MANAGER.AddProjectile(Skill::downcasted_shared_from_this<ProjectileSkill>(), objectInfo);
}
