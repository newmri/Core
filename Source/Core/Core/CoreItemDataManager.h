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

private:
	std::unordered_map<int32_t, CoreItemData> item;
};