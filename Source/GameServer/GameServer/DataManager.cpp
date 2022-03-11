#include "Include.h"

IMPLEMENT_SINGLETON(DataManager)

void DataManager::Init(void)
{
}

void DataManager::Release(void)
{
	GetInstance().~DataManager();
}

void DataManager::Load(void)
{
	CSV_LOAD_AND_TO_VECTOR("Data/CharacterCreateStat.csv", CharacterCreateStat, this->characterCreateStat);
	CSV_LOAD_AND_TO_VECTOR("Data/CharacterCreateGear.csv", CharacterCreateGear, this->characterCreateGear);
	CSV_LOAD_AND_TO_VECTOR("Data/CharacterAbilityByStat.csv", CharacterAbilityByStat, this->characterAbilityByStat);
}

CharacterInfo DataManager::MakeCharacterInfo(const uint8_t& level, const Define::Job& job)
{
	CharacterInfo info(level, job, this->characterCreateStat[job], this->characterCreateGear[job]);
	return info;
}