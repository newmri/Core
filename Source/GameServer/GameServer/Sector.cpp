#include "Include.h"

Sector::Sector()
{
	Init();
}

Sector::~Sector()
{

}

void Sector::Init(void)
{

}

void Sector::SetIndex(const NativeInfo::Vec2Int index)
{
	this->index = index;
}

void Sector::Add(std::shared_ptr<Creature> creature)
{
	if (IS_NULL(creature))
		return;

	Define::ObjectType objectType = creature->GetObjectType();

	switch (objectType)
	{
	case Define::ObjectType_PLAYER:
		this->playerList[creature->GetOID()] = dynamic_cast<Player*>(creature.get())->shared_from_this();
		break;
	default:
		break;
	}
}

void Sector::Remove(const Define::ObjectType objectType, const int64_t oid)
{
	switch (objectType)
	{
	case Define::ObjectType_PLAYER:
		this->playerList.erase(oid);
		break;
	default:
		break;
	}
}

std::shared_ptr<Player> Sector::FindPlayer(const int64_t& oid)
{
	auto iter = this->playerList.find(oid);
	if (IS_NOT_SAME(iter, this->playerList.end()))
		return iter->second;

	return nullptr;
}