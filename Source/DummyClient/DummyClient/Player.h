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
	Player(const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo, const PlayerType playerType);

public:
	PlayerType GetPlayerType(void);

public:
	virtual GamePacket::CharacterInfoT GetCharacterInfo(void);

public:
	virtual void SetState(const Define::ObjectState state) override;

public:
	virtual bool OnGetDamage(const GamePacket::DamageInfoT& damageInfo) override;

private:
	PlayerType playerType;
};