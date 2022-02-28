#pragma once

#define DATA_MANAGER GET_INSTANCE(DataManager)

struct CharacterCreateStat
{
	int16_t STR = 0;
	int16_t DEX = 0;
	int16_t INT = 0;
};

class DataManager
{
	DECLARE_SINGLETON(DataManager)

public:
	void Load(void);

public:
	CoreVector<CharacterCreateStat> characterCreateStat;
};