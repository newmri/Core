#include "Include.h"

OtherPlayer::OtherPlayer(const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo, const GamePacket::CharacterInfoT& characterInfo) :
	Player(objectInfoWithPos, creatureInfo, PlayerType::OTHER_PLAYER), characterInfo(characterInfo)
{
	Init();
}

OtherPlayer::~OtherPlayer()
{

}

void OtherPlayer::Init(void)
{

}

void OtherPlayer::Update(void)
{

}

void OtherPlayer::Clear(void)
{
	Player::Clear();
}

GamePacket::CharacterInfoT OtherPlayer::GetCharacterInfo(void)
{
	return this->characterInfo;
}

void OtherPlayer::SetState(const Define::ObjectState state)
{
	Player::SetState(state);
}
