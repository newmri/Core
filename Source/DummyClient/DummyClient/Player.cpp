#include "Include.h"

Player::Player(const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo, const PlayerType playerType) :
	Creature(objectInfoWithPos, creatureInfo), playerType(playerType)
{
	
}

PlayerType Player::GetPlayerType(void)
{
	return this->playerType;
}

Player::~Player()
{

}

void Player::Init(void)
{
	Creature::Init();
}

void Player::Update(void)
{
	Creature::Update();
}

void Player::Clear(void)
{
	Creature::Clear();
}

GamePacket::CharacterInfoT Player::GetCharacterInfo(void)
{
	return GamePacket::CharacterInfoT();
}

void Player::SetState(const Define::ObjectState state)
{
	Creature::SetState(state);
}

bool Player::OnGetDamage(const GamePacket::DamageInfoT& damageInfo)
{
	return Creature::OnGetDamage(damageInfo);
}
