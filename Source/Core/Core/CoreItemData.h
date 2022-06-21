#pragma once

#pragma pack(push, 1)

static const int32_t ITEM_EFFECT_MAX = 5;

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
	uint8_t usableMinLevel;
	uint8_t usableMaxLevel;
	Define::GradeType gradeType;
	int64_t coolTime;
	int32_t effectIndex[ITEM_EFFECT_MAX];
	bool isCashItem;
	int64_t buyPrice;
	int64_t sellPrice;
	uint16_t maxStackNum;
	uint16_t storage;
};

#pragma pack(pop)