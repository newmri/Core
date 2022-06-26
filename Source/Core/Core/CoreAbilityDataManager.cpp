#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreAbilityDataManager)

void CoreAbilityDataManager::Init(void)
{
}

void CoreAbilityDataManager::Release(void)
{
	GetInstance().~CoreAbilityDataManager();
}

void CoreAbilityDataManager::Load(void)
{
	CSV_LOAD_AND_TO_HASH_MAP("Data/Ability.csv", CoreAbilityData, this->ability, abilityID);
}

const CoreAbilityData* const CoreAbilityDataManager::GetAbilityData(const int32_t abilityID)
{
	auto iter = this->ability.find(abilityID);
	if (IS_SAME(iter, this->ability.end()))
		return nullptr;

	return &iter->second;
}

