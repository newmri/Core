#include "Include.h"

Player::Player(const int64_t& uid, const std::shared_ptr<CoreClientSession> session,
	const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo) :
	uid(uid), session(session), Creature(creatureInfo), characterInfo(characterInfo)
{
	Init();
}

Player::~Player()
{

}

void Player::Init(void)
{

}

void Player::Update(void)
{

}