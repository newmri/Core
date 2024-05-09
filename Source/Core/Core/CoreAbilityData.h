#pragma once

#pragma pack(push, 1)

struct CoreAbilityData
{
	CoreAbilityData()
	{

	}

	int32_t abilityID = 0;
	Define::AbilityType abilityType = Define::AbilityType::AbilityType_HP;
	int32_t abilityValue = 0;
};

#pragma pack(pop)
