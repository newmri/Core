#pragma once

class CoreGameDB : public CoreDB
{
public:
	CoreGameDB(std::wstring_view dbName, const uint8_t worldID, const uint8_t serverID);
	virtual ~CoreGameDB() override;

public:
	CoreItemUID GetItemUID(const uint16_t count);
	void AddItemToInventory(const ItemCreateSlotInfo& itemCreateSlotInfo, std::vector<Info::ItemSlotInfoT>& itemSlotInfoList);

public:
	bool OnLevelUp(const int64_t accountUID, const int64_t uid, const uint8_t newLevel, const int32_t newBonusStatPoint, const int32_t newHP, const int32_t newMP);
};