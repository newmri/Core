#include "Include.h"

Player::Player(const int64_t& uid, const std::shared_ptr<CoreClientSession> session, const std::shared_ptr<Character> character) :
	CoreUIDObj(uid), session(session), character(character)
{
	Init();
}

Player::~Player()
{

}

void Player::Init(void)
{

}