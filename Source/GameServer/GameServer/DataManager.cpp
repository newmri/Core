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
	CSV_LOAD_AND_TO_VECTOR("Data/CharacterAbilityByStat.csv", CharacterAbilityByStat, this->characterAbilityByStat);
}