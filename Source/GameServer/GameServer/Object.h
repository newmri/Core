#pragma once

class Object : public CoreGameObject, public inheritable_enable_shared_from_this<Object>
{
	OVERRIDE_GAME_OBJECT(Object)

public:
	Object(const Info::ObjectInfoT& objectInfo);

public:
	Info::ObjectInfoT GetObjectInfo(void);
	NativeInfo::PositionInfo GetPosInfo(void);
	Define::ObjectType GetObjectType(void) const;
	int64_t GetOID(void) const;
	int32_t GetMapID(void);
	NativeInfo::Vec2Int GetPos(void);
	Define::Dir GetDir(void);
	bool IsDead(void);
	Define::ObjectState GetState(void);

protected:
	NativeInfo::Vec2Int GetPosWithNoLock(void) const;
	Define::ObjectState GetStateWithNoLock(void) const;

public:
	void SetMove(const Define::ObjectState state, const NativeInfo::Vec2Int& destPos);
	virtual void SetState(const Define::ObjectState state);
	void SetDirection(const NativeInfo::Vec2Int& destPos);
	void SetPos(const NativeInfo::Vec2Int& pos);

protected:
	void SetStateWithNoLock(const Define::ObjectState state);
	void SetDirectionWithNoLock(const NativeInfo::Vec2Int& destPos);
	void SetPosWithNoLock(const NativeInfo::Vec2Int& pos);

public:
	virtual void MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet);
	void MakeMovePacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet);

protected:
	Info::ObjectInfoT objectInfo;

private:
	CACHE_ALIGN std::shared_mutex infoMutex;
};