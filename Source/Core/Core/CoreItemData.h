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
		uint32_t value : 32;
		int16_t serverID : 16;
		int16_t worldID : 16;
	};

	int64_t uid;
};

#pragma pack(pop)