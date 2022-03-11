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
}

NativeInfo::Stat DataManager::GetCharacterCreateStat(const Define::Job& job)
{
	return this->characterCreateStat[job];
}

void DataManager::GetCharacterCreateGear(const Define::Job& job, std::array<uint8_t, Define::GearType_END>& out)
{
	std::copy(this->characterCreateGear[job].index.begin(), this->characterCreateGear[job].index.end(), out.begin());
}