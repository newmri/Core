#include "Include.h"

Character::Character(const int64_t& accountUID, const int64_t& uid, const LoginPacket::CharacterInfoT& info) :
	CoreCharacter(accountUID, uid), info(info)
{

}

Character::~Character()
{

}