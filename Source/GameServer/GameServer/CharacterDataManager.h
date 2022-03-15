#pragma once

#define CHARACTER_DATA_MANAGER GET_INSTANCE(CharacterDataManager)

class CharacterDataManager
{
	DECLARE_SINGLETON(CharacterDataManager)

public:
	void Load(void);

public:
	void CalculateAbilityByStat(Info::CreatureInfoT& info);
	void CalculateSpeed(const Define::Job job, NativeInfo::Speed& speed);

public:
	CoreVector<CharacterAbilityByStat> characterAbilityByStat;
	CoreVector<NativeInfo::Speed> characterSpeed;
};