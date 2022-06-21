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
	void GetLevelData(const uint8_t level, LevelData& levelData);

private:
	CoreVector<CharacterAbilityByStat> characterAbilityByStat;
	CoreVector<NativeInfo::Speed> characterSpeed;
	std::unordered_map<int32_t, SkillData> skill;
	CoreVector<LevelData> level;
};