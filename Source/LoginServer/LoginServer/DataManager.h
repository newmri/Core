#pragma once

#define DATA_MANAGER GET_INSTANCE(DataManager)

class DataManager
{
	DECLARE_SINGLETON(DataManager)

public:
	void Load(void);

public:
	CoreVector<CharacterCreateStat> characterCreateStat;
	CoreVector<CharacterCreateGear> characterCreateGear;
};