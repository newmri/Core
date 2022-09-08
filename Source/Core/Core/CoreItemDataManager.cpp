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

bool CoreItemDataManager::IsValidGearType(const Define::GearType gearType)
{
	return IsBetween<Define::GearType>(gearType, Define::GearType_MIN, static_cast<Define::GearType>(Define::GearType_END - 1));
}

bool CoreItemDataManager::CanEquip(const CoreItemData* const itemData, const Define::Job job)
{
	if(!IsValidGearType(itemData->gearType))
		return false;

	if (IS_NOT_SAME(itemData->job, Define::Job_NONE) && IS_NOT_SAME(itemData->job, job))
		return false;

	return true;
}

bool CoreItemDataManager::CanUse(const CoreItemData* const itemData, const Define::Job job)
{
	if (IS_NOT_SAME(itemData->job, Define::Job_NONE) && IS_NOT_SAME(itemData->job, job))
		return false;

	switch (itemData->itemType)
	{
	case Define::ItemType_POTION:
		return true;
	default:
		return false;
	}
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

void CoreItemDataManager::DecreaseAbility(const int32_t itemID, NativeInfo::Ability& ability)
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

		ability.value[abilityData->abilityType] -= abilityData->abilityValue;
	}
}
