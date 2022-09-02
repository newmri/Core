#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreEffectDataManager)

void CoreEffectDataManager::Init(void)
{
}

void CoreEffectDataManager::Release(void)
{
	GetInstance().~CoreEffectDataManager();
}

void CoreEffectDataManager::Load(void)
{
	CSV_LOAD_AND_TO_HASH_MAP("Data/Effect.csv", CoreEffectData, this->effect, effectID);
}

const CoreEffectData* const CoreEffectDataManager::GetEffectData(const int32_t effectID)
{
	auto iter = this->effect.find(effectID);
	if (IS_SAME(iter, this->effect.end()))
		return nullptr;

	return &iter->second;
}

