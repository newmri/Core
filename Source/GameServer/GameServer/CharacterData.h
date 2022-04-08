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
	Define::Job job = Define::Job_MIN;
	Define::AbilityType damageType = Define::AbilityType_DAMAGE;
	Define::AbilityType criticalType = Define::AbilityType_CRITICAL_RATE;
	Define::AbilityType defenceType = Define::AbilityType_DEFENCE;
	int32_t damage = 0;
	Define::RangeDir rangeDir = Define::RangeDir_FRONT;
	uint8_t range = 0;
	int32_t coolTime = 0;
	int32_t HP = 0;
	int32_t MP = 0;
};

#pragma pack(pop)