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
	Info::ObjectInfoT objectInfo;
	objectInfo.pos_info = this->owner->GetPosInfo();

	switch (this->skillData.skillAnimationType)
	{
	case Define::SkillAnimationType_ATTACK_1:
		objectInfo.pos_info.pos += NativeInfo::Vec2Int(1, 2);
		break;
	}

	OBJECT_MANAGER.AddProjectile(this->owner, objectInfo);
}
