#pragma once

#define DATA_MANAGER GET_INSTANCE(DataManager)

class DataManager
{
	DECLARE_SINGLETON(DataManager)

public:
	void Load(void);

public:
	CharacterInfo MakeCharacterInfo(const uint8_t& level, const Define::Job& job);

public:
	CoreVector<CharacterCreateStat> characterCreateStat;
	CoreVector<CharacterCreateGear> characterCreateGear;
	CoreVector<CharacterAbilityByStat> characterAbilityByStat;
};