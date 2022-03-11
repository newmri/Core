#pragma once

class LoginCharacter : public CoreCharacter
{
public:
	LoginCharacter(const int64_t& accountUID, const int64_t& uid, const LoginPacket::CharacterInfoT& info);
	virtual ~LoginCharacter();

private:
	LoginPacket::CharacterInfoT info;
};