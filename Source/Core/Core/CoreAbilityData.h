#pragma once

#pragma pack(push, 1)

struct CoreAbilityData
{
	CoreAbilityData()
	{
		memset(this, 0, sizeof(this));
	}

	int32_t abilityID;
	Define::AbilityType abilityType;
	int32_t abilityValue;
};

#pragma pack(pop)
