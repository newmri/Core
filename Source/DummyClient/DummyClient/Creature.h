#pragma once

class Creature : public Object, public inheritable_enable_shared_from_this<Creature>
{
	OVERRIDE_GAME_OBJECT(Creature)

public:
	Creature(const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo);

public:
	Info::CreatureInfoT GetCreatureInfo(void);
	int32_t GetAbility(const Define::AbilityType abilityType);
	float GetSpeed(const Define::SpeedType speedType);

protected:
	float GetSpeedWithNoLock(const Define::SpeedType speedType) const;

public:
	bool UseHPMP(const int32_t HP, const int32_t MP);
	bool AddHP(const int32_t HP);
	std::tuple<int32_t, int32_t> GetHPMP(void);

public:
	virtual bool OnGetDamage(const GamePacket::DamageInfoT& damageInfo) override;

public:
	virtual void Revive(void);

protected:
	Info::CreatureInfoT creatureInfo;

protected:
	CACHE_ALIGN std::shared_mutex infoMutex;
	CACHE_ALIGN std::shared_mutex abilityMutex;

protected:
	CACHE_ALIGN std::shared_mutex skillMutex;
	std::map<int32_t, std::shared_ptr<Skill>> skillList;
};