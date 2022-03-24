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

GamePacket::CharacterInfoT Player::GetCharacterInfo(void)
{
	GamePacket::CharacterInfoT info;

	READ_LOCK(this->infoMutex);
	info.name = this->characterInfo.name;
	info.job = this->characterInfo.job;
	memcpy_s(&info.gear, sizeof(info.gear), &this->characterInfo.gear, sizeof(this->characterInfo.gear));
	return info;
}

void Player::MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
	auto creatureInfo = GetInfo();
	auto characterInfo = GetCharacterInfo();
	auto packedCreatureInfo = Info::CreatureInfo::Pack(PACKET_SEND_MANAGER.builder, &creatureInfo);
	auto packedCharacterInfo = GamePacket::CharacterInfo::Pack(PACKET_SEND_MANAGER.builder, &characterInfo);
	auto message = GamePacket::CreateSC_SPAWN_PLAYER_NOTI(PACKET_SEND_MANAGER.builder, packedCreatureInfo, packedCharacterInfo);
	packetType = GamePacket::Packet_SC_SPAWN_PLAYER_NOTI;
	packet = message.Union();
}

void Player::Send(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
	PACKET_SEND_MANAGER.Send(session, packetType, packet);
}