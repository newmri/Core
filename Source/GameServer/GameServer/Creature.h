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
	void AddHP(const int32_t HP);
	void AddHPWithNoLock(const int32_t HP);

	std::tuple<int32_t, int32_t> GetHPMP(void);

public:
	virtual void MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet) override;
	void MakeRevivePacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet);

public:
	void CalculateAbility(void);

protected:
	virtual void CalculateAbilityWithNoLock(void);

public:
	virtual void AddSkill(const int32_t skillID);
	void UseSkill(const int32_t skillID);

public:
	virtual bool OnGetDamage(GamePacket::DamageInfoT& damageInfo, const Define::AbilityType defenceType) override;

public:
	virtual void Revive(void);

protected:
	Info::CreatureInfoT creatureInfo;

protected:
	CACHE_ALIGN std::shared_mutex infoMutex;

protected:
	CACHE_ALIGN std::shared_mutex skillMutex;
	std::unordered_map<int32_t, std::shared_ptr<Skill>> skillList;

private:
	TIME_VALUE deadTime = 0;
};