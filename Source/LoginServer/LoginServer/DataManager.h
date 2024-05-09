#pragma once

#define DATA_MANAGER GET_INSTANCE(DataManager)

struct GearSlot
{
	std::array<int32_t, Define::GearType_END> itemID = {};
};

class DataManager
{
	DECLARE_SINGLETON(DataManager)

public:
	void Load(void);

public:
	NativeInfo::Stat GetCharacterCreateStat(const Define::Job& job);
	uint16_t GetCharacterCreateGear(const Define::Job& job, std::array<NativeInfo::GearSlotInfo, Define::GearType_END>& out);
	void GetCharacterCreateHPMP(const Define::Job& job, int32_t& HP, int32_t& MP);

private:
	CoreVector<NativeInfo::Stat> characterCreateStat;
	CoreVector<GearSlot> characterCreateGear;
	CoreVector<CharacterAbilityByStat> characterAbilityByStat;
};