#pragma once

class Character : public CoreCharacter
{
public:
	Character(const int64_t& accountUID, const GamePacket::MyCharacterInfoT& info);
	virtual ~Character();

public:
	void CalculateAbility(void);

private:
	GamePacket::MyCharacterInfoT info;

private:
	CACHE_ALIGN std::shared_mutex abilityMutex;
};