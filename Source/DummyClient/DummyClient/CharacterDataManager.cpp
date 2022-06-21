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
	CSV_LOAD_AND_TO_HASH_MAP("Data/CharacterSkill.csv", SkillData, this->skill, skillID);
}

const SkillData* const CharacterDataManager::GetSkillData(const int32_t skillID)
{
	auto iter = this->skill.find(skillID);
	if (IS_SAME(iter, this->skill.end()))
		return nullptr;

	return &iter->second;
}
