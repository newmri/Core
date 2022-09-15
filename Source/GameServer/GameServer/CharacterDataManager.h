#pragma once

#define CHARACTER_DATA_MANAGER GET_INSTANCE(CharacterDataManager)

class CharacterDataManager
{
	DECLARE_SINGLETON(CharacterDataManager)

public:
	void Load(void);

public:
	void CalculateAbilityByStat(Info::CreatureInfoT& info);
	void CalculateSpeed(const Define::Job job, NativeInfo::Speed& speed);
	const SkillData* const GetSkillData(const int32_t skillID);
	const LevelData* const GetLevelData(const uint8_t level);
	uint8_t GetMaxLevel(void);
	int32_t GetBonusStatPoint(uint8_t from, const uint8_t to);

private:
	std::vector<CharacterAbilityByStat> characterAbilityByStat;
	std::vector<NativeInfo::Speed> characterSpeed;
	std::unordered_map<int32_t, SkillData> skill;
	std::vector<LevelData> level;
};