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

void ProjectileSkill::DoAction(void)
{
	// 발사체 스폰
}
