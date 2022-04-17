#include "Include.h"

Player::Player(const int64_t& uid, const std::shared_ptr<CoreClientSession> session,
	const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo) :
	uid(uid), session(session), Creature(creatureInfo), characterInfo(characterInfo)
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
	PACKET_SEND_MANAGER.builder.Clear();
	auto packedCreatureInfo = Info::CreatureInfo::Pack(PACKET_SEND_MANAGER.builder, &creatureInfo);
	auto packedCharacterInfo = GamePacket::CharacterInfo::Pack(PACKET_SEND_MANAGER.builder, &characterInfo);
	auto message = GamePacket::CreateSC_SPAWN_PLAYER_NOTI(PACKET_SEND_MANAGER.builder, packedCreatureInfo, packedCharacterInfo);
	packetType = GamePacket::Packet_SC_SPAWN_PLAYER_NOTI;
	packet = message.Union();
}

void Player::Send(GamePacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	PACKET_SEND_MANAGER.Send(session, packetType, packet);
}

void Player::SetState(const Define::CreatureState state)
{
	Creature::SetState(state);

	if (IS_NOT_SAME(Define::CreatureState_WALK, state))
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

		float_t runSpeed = GetStateWithNoLock() == Define::CreatureState_RUN ? 1.3f : 1.0f;
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
