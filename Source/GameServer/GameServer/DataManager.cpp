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

void DataManager::CalculateAbilityByStat(GamePacket::MyCharacterInfoT& info)
{
	for (int32_t i = 0; i < Define::AbilityByStatType_END; ++i)
	{
		for (int32_t j = 0; j < Define::StatType_END; ++j)
		{
			info.ability.value[i] += info.stat.value[j] * this->characterAbilityByStat[j].multipleRate[i];
		}
	}
}