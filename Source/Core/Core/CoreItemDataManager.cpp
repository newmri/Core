#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreItemDataManager)

void CoreItemDataManager::Init(void)
{
}

void CoreItemDataManager::Release(void)
{
	GetInstance().~CoreItemDataManager();
}

void CoreItemDataManager::Load(void)
{
	CSV_LOAD_AND_TO_HASH_MAP("Data/Item.csv", CoreItemData, this->item, itemID);
}

const CoreItemData* const CoreItemDataManager::GetItemData(const int32_t itemID)
{
	auto iter = this->item.find(itemID);
	if (IS_SAME(iter, this->item.end()))
		return nullptr;

	return &iter->second;
}

void CoreItemDataManager::CalculateAbility(const NativeInfo::CharacterGear& gear, NativeInfo::Ability& ability)
{
	for (const auto& d : gear.info)
	{
		if (IS_SAME(0, d.itemID))
			continue;

		CalculateAbility(d.itemID, ability);
	}
}

void CoreItemDataManager::CalculateAbility(const int32_t itemID, NativeInfo::Ability& ability)
{
	auto itemData = GetItemData(itemID);
	if (IS_NULL(itemData))
		return;

	for (int32_t i = 0; i < Define::ItemAbility_MAX_COUNT; ++i)
	{
		if (IS_SAME(0, itemData->abilityID[i]))
			continue;

		auto abilityData = CORE_ABILITY_DATA_MANAGER.GetAbilityData(itemData->abilityID[i]);
		if (IS_NULL(abilityData))
			continue;

		ability.value[abilityData->abilityType] += abilityData->abilityValue;
	}
}
