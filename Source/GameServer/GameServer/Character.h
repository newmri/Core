#pragma once

class Character : public CoreCharacter
{
	OVERRIDE_OBJ_DELETE_DEFAULT_CONSTRUCTOR(Character)

public:
	Character(const int64_t& accountUID, const GamePacket::MyCharacterInfoT& info);

public:
	void CalculateAbility(void);

private:
	GamePacket::MyCharacterInfoT info;

private:
	CACHE_ALIGN std::shared_mutex abilityMutex;
};