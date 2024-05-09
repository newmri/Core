#pragma once

struct CharacterAbilityByStat
{
	CharacterAbilityByStat()
	{

	}

	std::array<uint8_t, Define::AbilityByStatType_END> multipleRate = {};
};