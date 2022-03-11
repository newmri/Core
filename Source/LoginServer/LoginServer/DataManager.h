#pragma once

#define DATA_MANAGER GET_INSTANCE(DataManager)

class DataManager
{
	DECLARE_SINGLETON(DataManager)

public:
	void Load(void);

public:
	NativeInfo::Stat GetCharacterCreateStat(const Define::Job& job);
	void GetCharacterCreateGear(const Define::Job& job, std::array<uint8_t, Define::GearType_END>& out);

private:
	CoreVector<NativeInfo::Stat> characterCreateStat;
	CoreVector<NativeInfo::CharacterGear> characterCreateGear;
};