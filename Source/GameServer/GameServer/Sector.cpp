#include "Include.h"

Sector::Sector(const NativeInfo::Vec2Int& index) : index(index)
{
}

void Sector::Add(std::shared_ptr<Creature> creature)
{
	this->creatureList[creature->GetOID()] = creature;
}

void Sector::Remove(const int64_t oid)
{
	this->creatureList.erase(oid);
}
