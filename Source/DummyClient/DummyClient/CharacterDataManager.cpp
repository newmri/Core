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

bool CharacterDataManager::GetSkillData(const int32_t skillID, SkillData& skillData)
{
	auto iter = this->skill.find(skillID);
	if (IS_SAME(iter, this->skill.end()))
		return false;

	memcpy_s(&skillData, sizeof(SkillData), iter->second.get(), sizeof(SkillData));
	return true;
}
