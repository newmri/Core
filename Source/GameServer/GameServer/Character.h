#pragma once

class Character : public CoreCharacter
{
public:
	Character(const int64_t& accountUID, const GamePacket::MyCharacterInfoT& info);
	virtual ~Character();

private:
	GamePacket::MyCharacterInfoT info;
};