#pragma once

#define CORE_ITEM_DATA_MANAGER GET_INSTANCE(CoreItemDataManager)

#include "CoreItemData.h"

class CoreItemDataManager
{
	DECLARE_SINGLETON(CoreItemDataManager)

public:
	void Load(const CoreItemUID itemUID);

public:
	const CoreItemData* const GetItemData(const int32_t itemID);

public:
	void CalculateAbility(const NativeInfo::CharacterGear& gear, NativeInfo::Ability& ability);
	void CalculateAbility(const int32_t itemID, NativeInfo::Ability& ability);

private:
	CACHE_ALIGN std::shared_mutex itemUIDMutex;
	CoreItemUID itemUID;

private:
	std::unordered_map<int32_t, CoreItemData> item;
};