#pragma once

class Projectile : public Object, public CoreMemoryPoolObj<Projectile, CORE_BIG_SIZE>
{
	OVERRIDE_GAME_OBJECT(Projectile)

public:
	Projectile(const std::shared_ptr<Creature> owner, const Info::ObjectInfoT& objectInfo);

protected:
	std::shared_ptr<Creature> owner;
};