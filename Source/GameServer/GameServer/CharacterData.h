#pragma once

struct CharacterAbilityByStat
{
	CharacterAbilityByStat()
	{
		memset(this, 0, sizeof(this));
	}

	uint8_t multipleRate[Define::AbilityByStatType_END];
};