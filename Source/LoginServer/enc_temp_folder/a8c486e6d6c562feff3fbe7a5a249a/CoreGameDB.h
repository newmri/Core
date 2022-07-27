#pragma once

class CoreGameDB : public CoreDB
{
public:
	CoreGameDB(std::wstring_view dbName, const uint8_t worldID, const uint8_t serverID);
	virtual ~CoreGameDB() override;

public:
	CoreItemUID GetItemUID(const uint16_t count);
	bool AddItemToInventory(const int32_t itemID, const uint16_t itemCount, const uint8_t needSlotCount, std::vector<Info::ItemSlotInfoT>& itemSlotInfoList);
};