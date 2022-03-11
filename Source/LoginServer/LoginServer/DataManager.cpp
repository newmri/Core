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
	CSV_LOAD_AND_TO_VECTOR("Data/CharacterCreateStat.csv", NativeInfo::Stat, this->characterCreateStat);
	CSV_LOAD_AND_TO_VECTOR("Data/CharacterCreateGear.csv", NativeInfo::CharacterGear, this->characterCreateGear);
	CSV_LOAD_AND_TO_VECTOR("Data/CharacterAbilityByStat.csv", CharacterAbilityByStat, this->characterAbilityByStat);
}

NativeInfo::Stat DataManager::GetCharacterCreateStat(const Define::Job& job)
{
	return this->characterCreateStat[job];
}

void DataManager::GetCharacterCreateGear(const Define::Job& job, std::array<uint8_t, Define::GearType_END>& out)
{
	std::copy(this->characterCreateGear[job].index.begin(), this->characterCreateGear[job].index.end(), out.begin());
}

void DataManager::GetCharacterCreateHPMP(const Define::Job& job, int32_t& HP, int32_t& MP)
{
	for (int i = 0; i < Define::StatType_END; ++i)
	{
		HP += this->characterCreateStat[job].value[i] * this->characterAbilityByStat[i].multipleRate[Define::AbilityByStatType_HP];
		MP += this->characterCreateStat[job].value[i] * this->characterAbilityByStat[i].multipleRate[Define::AbilityByStatType_MP];
	}
}