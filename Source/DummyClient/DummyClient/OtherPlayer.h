#pragma once

class OtherPlayer : public Player, public CoreMemoryPoolObj<OtherPlayer, CORE_BIG_SIZE>
{
	OVERRIDE_GAME_OBJECT(OtherPlayer)

public:
	OtherPlayer(const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo, const GamePacket::CharacterInfoT& characterInfo);

public:
	virtual GamePacket::CharacterInfoT GetCharacterInfo(void);

public:
	virtual void MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet) override;

public:
	virtual void SetState(const Define::ObjectState state) override;

public:
	virtual void AddSkill(const int32_t skillID) override;

private:
	GamePacket::CharacterInfoT characterInfo;
};