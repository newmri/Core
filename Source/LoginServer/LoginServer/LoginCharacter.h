#pragma once

class LoginCharacter : public CoreCharacter
{
public:
	LoginCharacter(const int64_t& accountUID, const int64_t& uid, const CharacterInfo& info);
	virtual ~LoginCharacter();

private:
	CharacterInfo info;
};