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
	int32_t GetMapID(void);
	NativeInfo::Vec2Int GetPos(void);
	float GetSpeed(const Define::SpeedType speedType);

protected:
	NativeInfo::Vec2Int GetPosWithNoLock(void) const;
	float GetSpeedWithNoLock(const Define::SpeedType speedType) const;

public:
	void SetMove(const Define::CreatureState state, const NativeInfo::Vec2Int& destPos);
	virtual void SetState(const Define::CreatureState state);
	void SetDirection(const NativeInfo::Vec2Int& destPos);
	void SetPos(const NativeInfo::Vec2Int& pos);

protected:
	void SetStateWithNoLock(const Define::CreatureState state);
	void SetDirectionWithNoLock(const NativeInfo::Vec2Int& destPos);
	void SetPosWithNoLock(const NativeInfo::Vec2Int& pos);

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