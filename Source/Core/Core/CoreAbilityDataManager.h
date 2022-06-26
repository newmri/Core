#pragma once

#define CORE_ABILITY_DATA_MANAGER GET_INSTANCE(CoreAbilityDataManager)

#include "CoreAbilityData.h"

class CoreAbilityDataManager
{
	DECLARE_SINGLETON(CoreAbilityDataManager)

public:
	void Load(void);

public:
	const CoreAbilityData* const GetAbilityData(const int32_t abilityID);

private:
	std::unordered_map<int32_t, CoreAbilityData> ability;
};