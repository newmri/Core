#pragma once

class Player : public Creature, public CoreMemoryPoolObj<Player, CORE_BIG_SIZE>
{
	OVERRIDE_GAME_OBJECT(Player)

public:
	Player(const int64_t& uid, const std::shared_ptr<CoreClientSession> session,
		const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo);

public:
	int64_t GetUID(void) const;

public:
	std::shared_ptr<CoreClientSession> GetSession(void);

public:
	GamePacket::CharacterInfoT GetCharacterInfo(void);

public:
	virtual void MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet) override;

protected:
	virtual void CalculateAbilityWithNoLock(void) override;

public:
	void Send(GamePacket::Packet packetType, flatbuffers::Offset<void> packet);

public:
	virtual void SetState(const Define::ObjectState state) override;

public:
	bool IsValidMoveSpeed(const NativeInfo::Vec2Int& destPos);

public:
	virtual void AddSkill(const int32_t skillID);

public:
	bool LoadData(void);

private:
	bool LoadItemInventory(void);

public:
	void SendItemInventoryInfo(void);

private:
	int64_t uid;
	std::shared_ptr<CoreClientSession> session;
	GamePacket::MyCharacterInfoT characterInfo;

private:
	CACHE_ALIGN std::shared_mutex itemInventoryMutex;
	CACHE_ALIGN uint8_t maxItemInventorySlotcount;
	CACHE_ALIGN std::unordered_map<int64_t, Info::ItemSlotInfoT> itemInventory;

private:
	TIME_VALUE lastMoveTime = 0;
};