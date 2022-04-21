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

void Sector::Add(std::shared_ptr<Object> object)
{
	if (IS_NULL(object))
		return;

	SendSpawnPacketToOldPlayer(object);

	Define::ObjectType objectType = object->GetObjectType();

	switch (objectType)
	{
	case Define::ObjectType_PLAYER:
	{
		auto player = std::dynamic_pointer_cast<Player>(object);
		SendSpawnPacketToNewPlayer(player);
		this->playerList[object->GetOID()] = player;
		break;
	}
	default:
		break;
	}
}

void Sector::Move(std::shared_ptr<Object> object)
{
	if (IS_NULL(object))
		return;

	GamePacket::Packet packetType;
	flatbuffers::Offset<void> packet;
	object->MakeMovePacket(packetType, packet);
	SendAllExceptMe(object->GetOID(), packetType, packet);
}

void Sector::Revive(std::shared_ptr<Creature> creature)
{
	if (IS_NULL(creature))
		return;

	GamePacket::Packet packetType;
	flatbuffers::Offset<void> packet;
	creature->MakeRevivePacket(packetType, packet);
	SendAll(packetType, packet);
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

void Sector::SendSpawnPacketToOldPlayer(std::shared_ptr<Object> object)
{
	GamePacket::Packet packetType;
	flatbuffers::Offset<void> packet;
	object->MakeSpawnPacket(packetType, packet);
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
		Sleep(1);
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

		Sleep(1);
	}
}

std::shared_ptr<Player> Sector::FindPlayer(const int64_t& oid)
{
	auto iter = this->playerList.find(oid);
	if (IS_NOT_SAME(iter, this->playerList.end()))
		return iter->second;

	return nullptr;
}

void Sector::GetCreatures(std::shared_ptr<Creature> creature, const Define::RangeDir& rangeDir, const uint8_t& range, CoreList<std::shared_ptr<Creature>>& creatureList, const bool liveCreatureOnly)
{
	auto cellPos = creature->GetPos();
	auto dir = creature->GetDir();

	switch (rangeDir)
	{
	case Define::RangeDir_FRONT:
		GetFrontCreatures(cellPos, dir, range, creatureList, liveCreatureOnly);
		break;
	case Define::RangeDir_BACK:
		GetFrontCreatures(cellPos, dir == Define::Dir_LEFT ? Define::Dir_RIGHT : Define::Dir_LEFT, range, creatureList, liveCreatureOnly);
		break;
	case Define::RangeDir_TWO_WAY:
		GetFrontCreatures(cellPos, dir, range, creatureList, liveCreatureOnly);
		GetFrontCreatures(cellPos, dir == Define::Dir_LEFT ? Define::Dir_RIGHT : Define::Dir_LEFT, range, creatureList, liveCreatureOnly);
		break;
	case Define::RangeDir_UP:
		GetFrontCreatures(cellPos, Define::Dir_UP, range, creatureList, liveCreatureOnly);
		break;
	case Define::RangeDir_DOWN:
		GetFrontCreatures(cellPos, Define::Dir_DOWN, range, creatureList, liveCreatureOnly);
		break;
	}
}

void Sector::GetFrontCreatures(NativeInfo::Vec2Int cellPos, const Define::Dir dir, const uint8_t& range, CoreList<std::shared_ptr<Creature>>& creatureList, const bool liveCreatureOnly)
{
	NativeInfo::Vec2Int increasePos;
	NativeInfo::Vec2Int destPos = cellPos.GetFrontPos(dir, range, increasePos);

	// Left or Right
	if (increasePos.x)
	{
		cellPos.x += increasePos.x;

		if (0 < increasePos.x)
		{
			for (; cellPos.x <= destPos.x; cellPos.x += increasePos.x)
				GetCreatures(cellPos, creatureList, liveCreatureOnly);
		}
		else
		{
			for (; cellPos.x >= destPos.x; cellPos.x += increasePos.x)
				GetCreatures(cellPos, creatureList, liveCreatureOnly);
		}
	}
	// Up or Down
	else
	{
		cellPos.y += increasePos.y;
		for (; cellPos.y <= destPos.y; cellPos.y += increasePos.y)
			GetCreatures(cellPos, creatureList, liveCreatureOnly);
	}
}

void Sector::GetCreatures(const NativeInfo::Vec2Int& cellPos, CoreList<std::shared_ptr<Creature>>& creatureList, const bool liveCreatureOnly)
{
	auto iter_begin = this->playerList.begin();
	auto iter_end = this->playerList.end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		if ((iter_begin->second)->GetPos() == cellPos && liveCreatureOnly && !(iter_begin->second)->IsDead())
			creatureList.push_back(iter_begin->second);
	}
}