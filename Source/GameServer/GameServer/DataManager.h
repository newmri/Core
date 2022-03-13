#pragma once

#define DATA_MANAGER GET_INSTANCE(DataManager)

class DataManager
{
	DECLARE_SINGLETON(DataManager)

public:
	void Load(void);

public:
	void CalculateAbilityByStat(Info::CreatureInfoT& info);

public:
	CoreVector<CharacterAbilityByStat> characterAbilityByStat;
};