#pragma once

#pragma pack(push, 1)

struct CoreItemData
{
	CoreItemData()
	{

	}

	int32_t itemID = 0;
	Define::Job job = Define::Job::Job_NONE;
	Define::ItemType itemType = Define::ItemType::ItemType_MIN;
	Define::GearType gearType = Define::GearType_NONE;
	Define::GradeType gradeType = Define::GradeType::GradeType_NORMAL;
	int64_t coolTime = 0;
	std::array<int32_t, Define::ItemAbility_MAX_COUNT> abilityID = {};
	std::array<int32_t, Define::ItemEffect_MAX_COUNT> effectID = {};
	bool isCashItem = false;
	int64_t buyPrice = 0;
	int64_t sellPrice = 0;
	uint16_t maxStackCount = 0;
	uint16_t storage = 0;
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

	}

	ItemCreateSlotInfo(const int64_t accountUID, const int64_t uid, const int32_t itemID, const uint16_t itemCount, const uint16_t maxStackCount, const uint8_t needSlotCount) :
		accountUID(accountUID), uid(uid), itemID(itemID), itemCount(itemCount), maxStackCount(maxStackCount), needSlotCount(needSlotCount)
	{

	}

	int64_t accountUID = 0;
	int64_t uid = 0;
	int32_t itemID = 0;
	uint16_t itemCount = 0;
	uint16_t maxStackCount = 0;
	uint8_t needSlotCount = 0;
};

#pragma pack(pop)