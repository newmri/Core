#pragma once

#define CORE_EFFECT_DATA_MANAGER GET_INSTANCE(CoreEffectDataManager)

#include "CoreEffectData.h"

class CoreEffectDataManager
{
	DECLARE_SINGLETON(CoreEffectDataManager)

public:
	void Load(void);

public:
	const CoreEffectData* const GetEffectData(const int32_t effectID);

private:
	std::unordered_map<int32_t, CoreEffectData> effect;
};