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
	CSV_LOAD_AND_TO_VECTOR("Data/CharacterCreateGear.csv", GearSlot, this->characterCreateGear);
	CSV_LOAD_AND_TO_VECTOR("Data/CharacterAbilityByStat.csv", CharacterAbilityByStat, this->characterAbilityByStat);
}

NativeInfo::Stat DataManager::GetCharacterCreateStat(const Define::Job& job)
{
	return this->characterCreateStat[job];
}

uint16_t DataManager::GetCharacterCreateGear(const Define::Job& job, std::array<NativeInfo::GearSlotInfo, Define::GearType_END>& out)
{
	uint16_t count = 0;
	for (int32_t i = 0; i < Define::GearType_END; ++i)
	{
		out[i].itemID = this->characterCreateGear[job].itemID[i];
		if (out[i].itemID)
			++count;
	}

	return count;
}

void DataManager::GetCharacterCreateHPMP(const Define::Job& job, int32_t& HP, int32_t& MP)
{
	for (int32_t i = 0; i < Define::StatType_END; ++i)
	{
		HP += this->characterCreateStat[job].value[i] * this->characterAbilityByStat[i].multipleRate[Define::AbilityByStatType_HP];
		MP += this->characterCreateStat[job].value[i] * this->characterAbilityByStat[i].multipleRate[Define::AbilityByStatType_MP];
	}
}