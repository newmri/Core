#pragma once

class Creature : public CoreGameObject, public std::enable_shared_from_this<Creature>
{
	OVERRIDE_GAME_OBJECT(Creature)

public:
	Creature(const Info::CreatureInfoT& creatureInfo);

public:
	Define::ObjectType GetObjectType(void) const;
	int64_t GetOID(void) const;
	int32_t GetMapID(void) const;
	NativeInfo::Vec2Int GetPos(void) const;

public:
	void SetPos(const NativeInfo::Vec2Int& pos);

public:
	void CalculateAbility(void);

protected:
	Info::CreatureInfoT creatureInfo;
	CACHE_ALIGN std::shared_mutex abilityMutex;
};