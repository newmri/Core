#pragma once


struct CharacterCreateStat
{
	CharacterCreateStat()
	{
		memset(this, 0, sizeof(this));
	}

	int16_t Stat[Define::StatType_MAX];
};

struct CharacterCreateGear
{
	CharacterCreateGear()
	{
		memset(this, 0, sizeof(this));
	}

	uint8_t Gear[Define::GearType_MAX];
};