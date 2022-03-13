#pragma once

class Creature : public CoreGameObj
{
	OVERRIDE_GAME_OBJ(Creature)

public:
	Creature(const Info::CreatureInfoT& creatureInfo);

public:
	void CalculateAbility(void);

protected:
	Info::CreatureInfoT creatureInfo;
	CACHE_ALIGN std::shared_mutex abilityMutex;
};