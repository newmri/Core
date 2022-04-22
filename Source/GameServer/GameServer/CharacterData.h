#pragma once

#pragma pack(push, 1)

struct CharacterAbilityByStat
{
	CharacterAbilityByStat()
	{
		memset(this, 0, sizeof(this));
	}

	uint8_t multipleRate[Define::AbilityByStatType_END];
};

struct SkillData
{
	int32_t skillID = 0;
	Define::SkillType skillType = Define::SkillType_NONE;
	Define::SkillAnimationType skillAnimationType = Define::SkillAnimationType_NONE;
	Define::Job job = Define::Job_MIN;
	TIME_VALUE skillHitTime = 0;
	Define::AbilityType damageType = Define::AbilityType_DAMAGE;
	int32_t damage = 0;
	Define::AbilityType criticalType = Define::AbilityType_CRITICAL_RATE;
	Define::AbilityType defenceType = Define::AbilityType_DEFENCE;
	Define::RangeDir rangeDir = Define::RangeDir_FRONT;
	uint8_t range = 0;
	int32_t coolTime = 0;
	int32_t HP = 0;
	int32_t MP = 0;
};

struct LevelData
{
	int64_t maxEXP;
	int32_t bonusPoint;
};

#pragma pack(pop)