#pragma once

enum class PlayerType
{
	MY_PLAYER,
	OTHER_PLAYER
};

class Player : public Creature
{
	OVERRIDE_GAME_OBJECT(Player)

public:
	Player(const Info::ObjectInfoT& objectInfo, const Info::CreatureInfoT& creatureInfo, const PlayerType playerType);

public:
	PlayerType GetPlayerType(void);

public:
	virtual GamePacket::CharacterInfoT GetCharacterInfo(void);

public:
	virtual void MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet) override;

public:
	virtual void SetState(const Define::ObjectState state) override;

public:
	virtual void AddSkill(const int32_t skillID) override;

private:
	PlayerType playerType;
};