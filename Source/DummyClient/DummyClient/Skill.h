#pragma once

class Object;
class Creature;

static const int32_t SKILL_MAX_RATE = 1000;

class Skill : public CoreObject, public inheritable_enable_shared_from_this<Skill>
{
	OVERRIDE_OBJECT(Skill)

public:
	Skill(const std::shared_ptr<Creature> owner, const SkillData& skillData);

public:
	bool UseSkill(void);

protected:
	virtual void DoAction(void);

public:
	virtual void DoDamage(std::shared_ptr<Object> target, std::vector<flatbuffers::Offset<GamePacket::DamageInfo>>& sendList);

public:
	Define::SkillType GetSkillType(void);
	int32_t GetMapID(void);
	NativeInfo::Vec2Int GetPos(void);

private:
	bool IsValidCoolTime(void);

protected:
	std::shared_ptr<Creature> owner;
	SkillData skillData;
	TIME_VALUE lastSkillUseTime = 0;
};