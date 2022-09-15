#include "Include.h"

IMPLEMENT_SINGLETON(CharacterDataManager)

void CharacterDataManager::Init(void)
{
}

void CharacterDataManager::Release(void)
{
	GetInstance().~CharacterDataManager();
}

void CharacterDataManager::Load(void)
{
	CSV_LOAD_AND_TO_VECTOR("Data/CharacterAbilityByStat.csv", CharacterAbilityByStat, this->characterAbilityByStat);
	CSV_LOAD_AND_TO_VECTOR("Data/CharacterCreateSpeedStat.csv", NativeInfo::Speed, this->characterSpeed);
	CSV_LOAD_AND_TO_HASH_MAP("Data/CharacterSkill.csv", SkillData, this->skill, skillID);
	CSV_LOAD_AND_TO_VECTOR("Data/CharacterLevel.csv", LevelData, this->level);
}

void CharacterDataManager::CalculateAbilityByStat(Info::CreatureInfoT& info)
{
	for (int32_t i = 0; i < Define::AbilityByStatType_END; ++i)
	{
		info.ability.value[i] = 0;
		for (int32_t j = 0; j < Define::StatType_END; ++j)
		{
			info.ability.value[i] += info.stat.value[j] * this->characterAbilityByStat[j].multipleRate[i];
		}
	}
}

void CharacterDataManager::CalculateSpeed(const Define::Job job, NativeInfo::Speed& speed)
{
	speed.value = this->characterSpeed[job].value;
}

const SkillData* const CharacterDataManager::GetSkillData(const int32_t skillID)
{
	auto iter = this->skill.find(skillID);
	if (IS_SAME(iter, this->skill.end()))
		return nullptr;

	return &iter->second;
}

const LevelData* const CharacterDataManager::GetLevelData(const uint8_t level)
{
	if (level > static_cast<uint8_t>(this->level.size()))
		return nullptr;

	return &this->level[level - 1];
}

uint8_t CharacterDataManager::GetMaxLevel(void)
{
	return this->level.size();
}

int32_t CharacterDataManager::GetBonusStatPoint(uint8_t from, const uint8_t to)
{
	int32_t bonusStatPoint = 0;

	if (from > to)
		return bonusStatPoint;

	for (; from <= to; ++from)
	{
		auto levelData = GetLevelData(from);
		if (IS_NULL(levelData))
			return bonusStatPoint;

		bonusStatPoint += levelData->bonusPoint;
	}

	return bonusStatPoint;
}
