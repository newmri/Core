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

void CharacterDataManager::GetLevelData(const uint8_t level, LevelData& levelData)
{
	levelData = this->level[level - 1];
}
