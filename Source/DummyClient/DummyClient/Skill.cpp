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
	if (!this->owner->UseHPMP(this->skillData.HP, this->skillData.MP))
		return;

	if (!IsValidCoolTime())
		return;

	{
		GAME_PACKET_SEND_MANAGER.Clear();
		auto message = GamePacket::CreateSC_USE_SKILL_RES(GAME_PACKET_SEND_MANAGER.builder, this->owner->GetObjectType(), this->owner->GetOID());
	}

	CORE_TIME_DELEGATE_MANAGER.Push(
		CoreTimeDelegate<>(std::bind(&Skill::DoAction, this), this->skillData.skillHitTime));
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
