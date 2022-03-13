#pragma once

// CharacterÀÇ uid´Â DB uid
class Character : public Creature, public CoreMemoryPoolObj<Character, CORE_BIG_SIZE>, public std::enable_shared_from_this<Character>
{
	OVERRIDE_GAME_OBJ(Character)

public:
	Character(const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo);

private:
	GamePacket::MyCharacterInfoT characterInfo;
};