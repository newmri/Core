#pragma once

class Creature : public CoreGameObject, public std::enable_shared_from_this<Creature>
{
	OVERRIDE_GAME_OBJECT(Creature)

public:
	Creature(const Info::CreatureInfoT& creatureInfo);

public:
	Info::CreatureInfoT GetInfo(void);
	Define::ObjectType GetObjectType(void) const;
	int64_t GetOID(void) const;
	int32_t GetMapID(void) const;
	NativeInfo::Vec2Int GetPos(void) const;

public:
	void SetDirection(const NativeInfo::Vec2Int& destPos);
	void SetPos(const NativeInfo::Vec2Int& pos);

public:
	virtual void MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet);
	void MakeMovePacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet);

public:
	void CalculateAbility(void);

protected:
	Info::CreatureInfoT creatureInfo;

	CACHE_ALIGN std::shared_mutex infoMutex;
	CACHE_ALIGN std::shared_mutex abilityMutex;
};