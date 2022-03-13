#pragma once

class Character : public CoreCharacter
{
public:
	Character(const int64_t& accountUID, const int64_t& uid, const LoginPacket::CharacterInfoT& info);
	virtual ~Character();

private:
	LoginPacket::CharacterInfoT info;
};