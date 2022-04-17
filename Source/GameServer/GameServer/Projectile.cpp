#include "Include.h"

Projectile::~Projectile()
{

}

void Projectile::Init(void)
{

}

void Projectile::Update(void)
{

}

Projectile::Projectile(const std::shared_ptr<Creature> owner, const Info::CreatureInfoT& creatureInfo) : owner(owner), Creature(creatureInfo)
{
}
