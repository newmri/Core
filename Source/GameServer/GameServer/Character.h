#pragma once

class Character : public CoreCharacter
{
public:
	Character(const int64_t& accountUID, const int64_t& uid, const CharacterInfo& info);
	virtual ~Character();

private:
	CharacterInfo info;
};