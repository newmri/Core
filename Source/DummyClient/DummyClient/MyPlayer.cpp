#include "Include.h"

MyPlayer::MyPlayer(const std::shared_ptr<CoreServerSession> session,
	const Info::ObjectInfoT& objectInfo, const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo) :
	session(session), Player(objectInfo, creatureInfo, PlayerType::MY_PLAYER), characterInfo(characterInfo)
{
	Init();
}

std::shared_ptr<CoreServerSession> MyPlayer::GetSession(void)
{
	return this->session->shared_from_this();
}

MyPlayer::~MyPlayer()
{

}

void MyPlayer::Init(void)
{

}

void MyPlayer::Update(void)
{

}

GamePacket::CharacterInfoT MyPlayer::GetCharacterInfo(void)
{
	GamePacket::CharacterInfoT info;

	READ_LOCK(this->infoMutex);
	info.name = this->characterInfo.name;
	info.job = this->characterInfo.job;
	memcpy_s(&info.gear, sizeof(info.gear), &this->characterInfo.gear, sizeof(this->characterInfo.gear));
	return info;
}

void MyPlayer::MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
	auto objectInfo = GetObjectInfo();
	auto creatureInfo = GetCreatureInfo();
	auto characterInfo = GetCharacterInfo();

	GAME_PACKET_SEND_MANAGER.Clear();
	auto packedObjectInfo = Info::ObjectInfo::Pack(GAME_PACKET_SEND_MANAGER.builder, &objectInfo);
	auto packedCreatureInfo = Info::CreatureInfo::Pack(GAME_PACKET_SEND_MANAGER.builder, &creatureInfo);
	auto packedCharacterInfo = GamePacket::CharacterInfo::Pack(GAME_PACKET_SEND_MANAGER.builder, &characterInfo);
	auto message = GamePacket::CreateSC_SPAWN_PLAYER_NOTI(GAME_PACKET_SEND_MANAGER.builder, packedObjectInfo, packedCreatureInfo, packedCharacterInfo);

	packetType = GamePacket::Packet_SC_SPAWN_PLAYER_NOTI;
	packet = message.Union();
}

void MyPlayer::Send(GamePacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	GAME_PACKET_SEND_MANAGER.Send(this->session, packetType, packet);
}

void MyPlayer::SetState(const Define::ObjectState state)
{
	Player::SetState(state);

	if (IS_NOT_SAME(Define::ObjectState_WALK, state))
	{
		this->lastMoveTime = 0;
	}
}

void MyPlayer::AddSkill(const int32_t skillID)
{
	WRITE_LOCK(this->skillMutex);

	SkillData skillData;
	if (!CHARACTER_DATA_MANAGER.GetSkillData(skillID, skillData))
		return;

	switch (skillData.skillType)
	{
	case Define::SkillType_NORMAL:
		this->skillList[skillID] = std::make_shared<Skill>(Object::downcasted_shared_from_this<Creature>(), skillData);
		break;
	case Define::SkillType_ARROW:
		this->skillList[skillID] = std::make_shared<ProjectileSkill>(Object::downcasted_shared_from_this<Creature>(), skillData);
		break;
	}
}
