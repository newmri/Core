#include "Include.h"

MyPlayer::MyPlayer(const std::shared_ptr<CoreServerSession> session,
	const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo) :
	session(session), Player(objectInfoWithPos, creatureInfo, PlayerType::MY_PLAYER), characterInfo(characterInfo)
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
	CORE_TIME_DELEGATE_MANAGER.Push(
		CoreTimeDelegate<>(
			std::bind(&MyPlayer::UseSkill, this),
			CORE_RANDOM_MANAGER_TIME.GetRandom(SEC, SEC * 10)));
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

void MyPlayer::UseSkill(void)
{
	CORE_TIME_DELEGATE_MANAGER.Push(
		CoreTimeDelegate<>(
			std::bind(&MyPlayer::UseSkill, this),
			CORE_RANDOM_MANAGER_TIME.GetRandom(SEC, SEC * 10)));

	if (IsDead())
		return;

	READ_LOCK(this->skillMutex);

	if (this->skillList.empty())
		return;

	int32_t offset = CORE_RANDOM_MANAGER_INT.GetRandom(0, static_cast<int32_t>(this->skillList.size() - 1));
	auto iter = this->skillList.begin();

	for (int32_t i = 0; i < offset; ++i)
		++iter;

	if (iter->second->UseSkill())
	{
		GAME_PACKET_SEND_MANAGER.Clear();
		auto message = GamePacket::CreateCS_USE_SKILL_REQ(GAME_PACKET_SEND_MANAGER.builder, iter->first);
		GAME_PACKET_SEND_MANAGER.Send(this->session, GamePacket::Packet_CS_USE_SKILL_REQ, message.Union());
	}

}

bool MyPlayer::OnGetDamage(const GamePacket::DamageInfoT& damageInfo)
{
	if (Player::OnGetDamage(damageInfo))
	{
		CORE_TIME_DELEGATE_MANAGER.Push(
			CoreTimeDelegate<>(
				std::bind(&MyPlayer::SendReviveReq, this),
				this->reviveTime, this->reviveTime, 2));

		return true;
	}

	return false;
}

void MyPlayer::SendReviveReq(void)
{
	if (IsDead())
	{
		GAME_PACKET_SEND_MANAGER.Clear();
		auto message = GamePacket::CreateCS_REVIVE_REQ(GAME_PACKET_SEND_MANAGER.builder);
		GAME_PACKET_SEND_MANAGER.Send(session, GamePacket::Packet_CS_REVIVE_REQ, message.Union());
	}
}
