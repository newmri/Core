#include "Include.h"

Player::Player(const Info::ObjectInfoT& objectInfo, const Info::CreatureInfoT& creatureInfo, const PlayerType playerType) :
	Creature(objectInfo, creatureInfo), playerType(playerType)
{
	Init();
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

}

void Player::Update(void)
{

}

GamePacket::CharacterInfoT Player::GetCharacterInfo(void)
{
	return GamePacket::CharacterInfoT();
}

void Player::MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{

}

void Player::SetState(const Define::ObjectState state)
{
	Creature::SetState(state);
}

void Player::AddSkill(const int32_t skillID)
{
	
}
