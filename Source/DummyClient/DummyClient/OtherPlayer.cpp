#include "Include.h"

OtherPlayer::OtherPlayer(const Info::ObjectInfoT& objectInfo, const Info::CreatureInfoT& creatureInfo, const GamePacket::CharacterInfoT& characterInfo) :
	Player(objectInfo, creatureInfo, PlayerType::OTHER_PLAYER), characterInfo(characterInfo)
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

GamePacket::CharacterInfoT OtherPlayer::GetCharacterInfo(void)
{
	READ_LOCK(this->infoMutex);
	return this->characterInfo;
}

void OtherPlayer::MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{

}
void OtherPlayer::SetState(const Define::ObjectState state)
{
	Player::SetState(state);
}

void OtherPlayer::AddSkill(const int32_t skillID)
{

}
