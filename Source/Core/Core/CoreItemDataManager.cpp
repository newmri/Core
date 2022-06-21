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
