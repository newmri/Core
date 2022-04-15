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
	bool GetSkillData(const int32_t skillID, SkillData& skillData);
	void GetLevelData(const uint8_t level, LevelData& levelData);

private:
	CoreVector<CharacterAbilityByStat> characterAbilityByStat;
	CoreVector<NativeInfo::Speed> characterSpeed;
	std::unordered_map<int32_t, std::shared_ptr<SkillData>> skill;
	CoreVector<LevelData> level;
};