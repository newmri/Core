#pragma once

class CoreGameDB : public CoreDB
{
public:
	CoreGameDB() = default;
	CoreGameDB(DBInfo&& dbInfo);
	virtual ~CoreGameDB() override;

public:
	void AddItemToInventory(const ItemCreateSlotInfo& itemCreateSlotInfo, std::vector<Info::ItemSlotInfoT>& itemSlotInfoList);
	bool UpdateInventoryItemCount(const int64_t accountUID, const int64_t uid, Info::ItemSlotInfoT* itemSlotInfo, const uint16_t useCount);
	bool DeleteInventoryItem(const int64_t accountUID, const int64_t uid, Info::ItemSlotInfoT* itemSlotInfo);

public:
	bool OnLevelUp(const int64_t accountUID, const int64_t uid, const uint8_t newLevel, const int32_t newBonusStatPoint, const int32_t newHP, const int32_t newMP);
};