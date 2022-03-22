#pragma once

class Sector : CoreObject
{
	OVERRIDE_OBJECT(Sector)

public:
	void SetIndex(const NativeInfo::Vec2Int index);

public:
	void Add(std::shared_ptr<Creature> creature);
	void Remove(const Define::ObjectType objectType, const int64_t oid);

private:
	void SendSpawnPacket(std::shared_ptr<Creature> creature);
	void Send(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet);

public:
	std::shared_ptr<Player> FindPlayer(const int64_t& oid);

private:
	NativeInfo::Vec2Int index;
	std::map<int64_t, std::shared_ptr<Player>> playerList;
};