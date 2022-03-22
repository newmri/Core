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

	SendSpawnPacket(creature);

	Define::ObjectType objectType = creature->GetObjectType();

	switch (objectType)
	{
	case Define::ObjectType_PLAYER:
		this->playerList[creature->GetOID()] = std::dynamic_pointer_cast<Player>(creature);
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

void Sector::SendSpawnPacket(std::shared_ptr<Creature> creature)
{
	GamePacket::Packet packetType;
	flatbuffers::Offset<void> packet;
	creature->MakeSpawnPacket(packetType, packet);
	Send(packetType, packet);
}

void Sector::Send(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
	auto iter_begin = this->playerList.begin();
	auto iter_end = this->playerList.end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		(iter_begin->second)->Send(packetType, packet);
	}
}

std::shared_ptr<Player> Sector::FindPlayer(const int64_t& oid)
{
	auto iter = this->playerList.find(oid);
	if (IS_NOT_SAME(iter, this->playerList.end()))
		return iter->second;

	return nullptr;
}