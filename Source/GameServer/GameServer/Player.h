#pragma once

class Player : public Creature, public CoreMemoryPoolObj<Player, CORE_BIG_SIZE>
{
	OVERRIDE_GAME_OBJECT(Player)

public:
	Player(const int64_t& uid, const std::shared_ptr<CoreClientSession> session,
		const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo);

public:
	GamePacket::CharacterInfoT GetCharacterInfo(void);

public:
	virtual void MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet) override;

public:
	void Send(GamePacket::Packet packetType, flatbuffers::Offset<void> packet);

private:
	int64_t uid;
	std::shared_ptr<CoreClientSession> session;
	GamePacket::MyCharacterInfoT characterInfo;
};