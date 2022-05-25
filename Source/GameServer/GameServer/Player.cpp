#include "Include.h"

Player::Player(const int64_t& uid, const std::shared_ptr<CoreClientSession> session,
	const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo) :
	uid(uid), session(session), Creature(objectInfoWithPos, creatureInfo), characterInfo(characterInfo)
{
	Init();
}

int64_t Player::GetUID(void) const
{
	return this->uid;
}

std::shared_ptr<CoreClientSession> Player::GetSession(void)
{
	return this->session->shared_from_this();
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

void Player::Clear(void)
{
	Creature::Clear();
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
	auto objectInfoWithPos = GetObjectInfoWithPos();
	auto creatureInfo = GetCreatureInfo();
	auto characterInfo = GetCharacterInfo();

	PACKET_SEND_MANAGER.Clear();
	auto packedObjectInfoWithPos = Info::ObjectInfoWithPos::Pack(PACKET_SEND_MANAGER.builder, &objectInfoWithPos);
	auto packedCreatureInfo = Info::CreatureInfo::Pack(PACKET_SEND_MANAGER.builder, &creatureInfo);
	auto packedCharacterInfo = GamePacket::CharacterInfo::Pack(PACKET_SEND_MANAGER.builder, &characterInfo);
	auto message = GamePacket::CreateSC_SPAWN_PLAYER_NOTI(PACKET_SEND_MANAGER.builder, packedObjectInfoWithPos, packedCreatureInfo, packedCharacterInfo);

	packetType = GamePacket::Packet_SC_SPAWN_PLAYER_NOTI;
	packet = message.Union();
}

void Player::Send(GamePacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	PACKET_SEND_MANAGER.Send(this->session, packetType, packet);
}

void Player::SetState(const Define::ObjectState state)
{
	Creature::SetState(state);

	if (IS_NOT_SAME(Define::ObjectState_WALK, state))
	{
		this->lastMoveTime = 0;
	}
}

bool Player::IsValidMoveSpeed(const NativeInfo::Vec2Int& destPos)
{
	int32_t clientDist = 0;
	float_t moveSpeed = 0.0f;
	{
		READ_LOCK(this->infoMutex);
		NativeInfo::Vec2Int dir = destPos - GetPosWithNoLock();

		// 한칸씩 이동 가능
		clientDist = dir.GetMagnitude();
		if (clientDist > 1)
			return false;

		float_t runSpeed = GetStateWithNoLock() == Define::ObjectState_RUN ? 1.3f : 1.0f;
		moveSpeed = GetSpeedWithNoLock(Define::SpeedType_MOVE_SPEED) * runSpeed;
	}

	// 첫 이동
	if (IS_SAME(0, this->lastMoveTime))
	{
		lastMoveTime = CORE_TIME_MANAGER.GetNowMilliSeconds();
		return true;
	}

	TIME_VALUE nowTime = CORE_TIME_MANAGER.GetNowMilliSeconds();
	TIME_VALUE timeDiff = nowTime - this->lastMoveTime;
	int32_t serverDist = CoreUtil::IntRound(moveSpeed * (timeDiff / SEC_FLOAT));

	if (clientDist > serverDist)
	{
		CORE_LOG.Log(LogType::LOG_HACK, GetOID(), "ClientDist: " + TO_STR(clientDist) + "ServerDist: " + TO_STR(serverDist));
		return false;
	}

	this->lastMoveTime = nowTime;
	return true;
}

void Player::AddSkill(const int32_t skillID)
{
	SkillData skillData;
	if (!CHARACTER_DATA_MANAGER.GetSkillData(skillID, skillData))
		return;

	WRITE_LOCK(this->skillMutex);

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
