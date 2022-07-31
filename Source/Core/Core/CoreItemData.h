#pragma once

#pragma pack(push, 1)

struct CoreItemData
{
	CoreItemData()
	{
		memset(this, 0, sizeof(this));
	}

	int32_t itemID;
	Define::Job job;
	Define::ItemType itemType;
	Define::GearType gearType;
	Define::GradeType gradeType;
	int64_t coolTime;
	int32_t abilityID[Define::ItemAbility_MAX_COUNT];
	bool isCashItem;
	int64_t buyPrice;
	int64_t sellPrice;
	uint16_t maxStackCount;
	uint16_t storage;
};

union CoreItemUID
{
	struct
	{
		int64_t value : 48,
				serverID : 8,
				worldID : 8;
	};

	int64_t uid = 0;
};

struct ItemCreateSlotInfo
{
	ItemCreateSlotInfo()
	{
		memset(this, 0, sizeof(this));
	}

	ItemCreateSlotInfo(const int64_t accountUID, const int64_t uid, const int32_t itemID, const uint16_t itemCount, const uint16_t maxStackCount, const uint8_t needSlotCount) :
		accountUID(accountUID), uid(uid), itemID(itemID), itemCount(itemCount), maxStackCount(maxStackCount), needSlotCount(needSlotCount)
	{

	}

	int64_t accountUID;
	int64_t uid;
	int32_t itemID;
	uint16_t itemCount;
	uint16_t maxStackCount;
	uint8_t needSlotCount;
};

#pragma pack(pop)