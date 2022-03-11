#pragma once

struct Stat
{
	Stat()
	{
		memset(this, 0, sizeof(this));
	}

	int16_t value[Define::StatType_STAT_END];
};

struct Ability
{
	Ability()
	{
		memset(this, 0, sizeof(this));
	}

	int32_t value[Define::Ability_ABILITY_END];
};

struct CharacterGear
{
	CharacterGear()
	{
		memset(this, 0, sizeof(this));
	}

	uint8_t value[Define::GearType_GEAR_END];
};

struct CharacterInfo
{
	CharacterInfo()
	{
		memset(this, 0, sizeof(this));
	}

	CharacterInfo(const uint8_t& level, const Define::Job& job, const Stat& stat, const CharacterGear& gear) :
		level(level),
		job(job),
		stat(stat),
		gear(gear)
	{

	}

	uint8_t level;
	uint8_t job;
	Stat stat;
	Ability ability;
	CharacterGear gear;
};

struct CharacterLoadInfo
{
	int64_t uid = 0;
	wchar_t name[Define::CharacterLimit_MaxNameLen];
	CharacterInfo info;
};

struct CharacterAbilityByStat
{
	CharacterAbilityByStat()
	{
		memset(this, 0, sizeof(this));
	}

	uint8_t multiplyValue[Define::AbilityByStatType_ABILITY_BY_STAT_END];
};