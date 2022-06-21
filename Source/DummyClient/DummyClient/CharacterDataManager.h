#pragma once

#define CHARACTER_DATA_MANAGER GET_INSTANCE(CharacterDataManager)

class CharacterDataManager
{
	DECLARE_SINGLETON(CharacterDataManager)

public:
	void Load(void);

public:
	const SkillData* const GetSkillData(const int32_t skillID);

private:
	std::unordered_map<int32_t, SkillData> skill;
};