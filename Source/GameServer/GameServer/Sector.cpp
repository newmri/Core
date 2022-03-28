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

	SendSpawnPacketToOldPlayer(creature);

	Define::ObjectType objectType = creature->GetObjectType();

	switch (objectType)
	{
	case Define::ObjectType_PLAYER:
	{
		auto player = std::dynamic_pointer_cast<Player>(creature);
		SendSpawnPacketToNewPlayer(player);
		this->playerList[creature->GetOID()] = player;
		break;
	}
	default:
		break;
	}
}

void Sector::Move(std::shared_ptr<Creature> creature)
{
	if (IS_NULL(creature))
		return;

	GamePacket::Packet packetType;
	flatbuffers::Offset<void> packet;
	creature->MakeMovePacket(packetType, packet);
	SendAllExceptMe(creature->GetOID(), packetType, packet);
}

void Sector::Remove(const Define::ObjectType objectType, const int64_t& oid)
{
	switch (objectType)
	{
	case Define::ObjectType_PLAYER:
		this->playerList.erase(oid);
		break;
	default:
		break;
	}

	SendDespawnPacket(objectType, oid);
}

void Sector::SendSpawnPacketToOldPlayer(std::shared_ptr<Creature> creature)
{
	GamePacket::Packet packetType;
	flatbuffers::Offset<void> packet;
	creature->MakeSpawnPacket(packetType, packet);
	SendAll(packetType, packet);
}

void Sector::SendSpawnPacketToNewPlayer(std::shared_ptr<Player> player)
{
	auto iter_begin = this->playerList.begin();
	auto iter_end = this->playerList.end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		GamePacket::Packet packetType;
		flatbuffers::Offset<void> packet;
		(iter_begin->second)->MakeSpawnPacket(packetType, packet);
		player->Send(packetType, packet);
	}
}

void Sector::SendDespawnPacket(const Define::ObjectType objectType, const int64_t& oid)
{
	GamePacket::Packet packetType = GamePacket::Packet_SC_DESPAWN_OBJECT_NOTI;
	flatbuffers::Offset<void> packet;
	PACKET_SEND_MANAGER.builder.Clear();
	auto message = GamePacket::CreateSC_DESPAWN_OBJECT_NOTI(PACKET_SEND_MANAGER.builder, objectType, oid);
	packet = message.Union();
	SendAll(packetType, packet);
}

void Sector::SendAll(GamePacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	auto iter_begin = this->playerList.begin();
	auto iter_end = this->playerList.end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		(iter_begin->second)->Send(packetType, packet);
	}
}

void Sector::SendAllExceptMe(const int64_t& oid, GamePacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	auto iter_begin = this->playerList.begin();
	auto iter_end = this->playerList.end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		if((iter_begin->second)->GetOID() != oid)
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