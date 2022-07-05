#pragma once

#define CORE_ITEM_DATA_MANAGER GET_INSTANCE(CoreItemDataManager)

#include "CoreItemData.h"

class CoreItemDataManager
{
	DECLARE_SINGLETON(CoreItemDataManager)

public:
	void Load(void);

public:
	const CoreItemData* const GetItemData(const int32_t itemID);

public:
	bool IsValidGearType(const Define::GearType gearType);

public:
	void CalculateAbility(const NativeInfo::CharacterGear& gear, NativeInfo::Ability& ability);
	void CalculateAbility(const int32_t itemID, NativeInfo::Ability& ability);
	void DecreaseAbility(const int32_t itemID, NativeInfo::Ability& ability);

private:
	std::unordered_map<int32_t, CoreItemData> item;
};