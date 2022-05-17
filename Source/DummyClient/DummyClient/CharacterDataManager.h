#pragma once

#define CHARACTER_DATA_MANAGER GET_INSTANCE(CharacterDataManager)

class CharacterDataManager
{
	DECLARE_SINGLETON(CharacterDataManager)

public:
	void Load(void);

public:
	bool GetSkillData(const int32_t skillID, SkillData& skillData);

private:
	std::unordered_map<int32_t, std::shared_ptr<SkillData>> skill;
};