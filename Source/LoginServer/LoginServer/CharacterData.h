#pragma once

struct CharacterCreateStat
{
	CharacterCreateStat()
	{
		memset(this, 0, sizeof(this));
	}

	int16_t value[Define::StatType_STAT_END];
};

struct CharacterCreateGear
{
	CharacterCreateGear()
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

	CharacterInfo(const uint8_t& level, const Define::Job& job, const CharacterCreateStat& stat, const CharacterCreateGear& gear) :
		level(level),
		job(job),
		stat(stat),
		gear(gear)
	{

	}

	uint8_t level;
	uint8_t job;
	CharacterCreateStat stat;
	CharacterCreateGear gear;
};

struct CharacterLoadInfo
{
	int64_t uid = 0;
	wchar_t name[Define::CharacterLimit_MaxNameLen];
	CharacterInfo info;
};