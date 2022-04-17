#pragma once

class Projectile : public Creature, public CoreMemoryPoolObj<Projectile, CORE_BIG_SIZE>
{
	OVERRIDE_GAME_OBJECT(Projectile)

public:
	Projectile(const std::shared_ptr<Creature> owner, const Info::CreatureInfoT& creatureInfo);

protected:
	std::shared_ptr<Creature> owner;
	Info::CreatureInfoT creatureInfo;
};