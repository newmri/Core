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
			std::bind(&MyPlayer::DoAI, this),
			CORE_RANDOM_MANAGER_TIME.GetRandom(this->aiMinTime, this->aiMaxTime)));
}

void MyPlayer::Update(void)
{

}

void MyPlayer::Clear(void)
{
	Player::Clear();
}

GamePacket::CharacterInfoT MyPlayer::GetCharacterInfo(void)
{
	GamePacket::CharacterInfoT info;

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
	const SkillData* const skillData = CHARACTER_DATA_MANAGER.GetSkillData(skillID);
	if (IS_NULL(skillData))
		return;

	WRITE_LOCK(this->skillMutex);

	switch (skillData->skillType)
	{
	case Define::SkillType_NORMAL:
		this->skillList[skillID] = std::make_shared<Skill>(Object::downcasted_shared_from_this<Creature>(), *skillData);
		break;
	case Define::SkillType_ARROW:
		this->skillList[skillID] = std::make_shared<ProjectileSkill>(Object::downcasted_shared_from_this<Creature>(), *skillData);
		break;
	}
}

void MyPlayer::UseSkill(void)
{
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
		DoAI();
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

bool MyPlayer::Revive(const Info::ObjectInfoWithPosT& objectInfoWithPos)
{
	if (!Creature::Revive(objectInfoWithPos))
		return false;

	return true;
}

void MyPlayer::DoAI(void)
{
	if (IsDead())
	{
		SendReviveReq();

		CORE_TIME_DELEGATE_MANAGER.Push(
			CoreTimeDelegate<>(
				std::bind(&MyPlayer::DoAI, Object::downcasted_shared_from_this<MyPlayer>()),
				CORE_RANDOM_MANAGER_TIME.GetRandom(this->aiMinTime, this->aiMaxTime)));

		return;
	}

	static int32_t stateMin = Define::ObjectState_MIN;
	static int32_t stateMax = Define::ObjectState_MAX;

	auto currState = GetState();
	Define::ObjectState state = static_cast<Define::ObjectState>(CORE_RANDOM_MANAGER_INT.GetRandom(stateMin, stateMax));

	if (IS_NOT_SAME(currState, state))
	{
		switch (state)
		{
		case Define::ObjectState_IDLE:
		{
			SetState(state);

			GAME_PACKET_SEND_MANAGER.Clear();
			auto message = GamePacket::CreateCS_SET_STATE_REQ(GAME_PACKET_SEND_MANAGER.builder, state);
			GAME_PACKET_SEND_MANAGER.Send(session, GamePacket::Packet_CS_SET_STATE_REQ, message.Union());
		}
			break;
		case Define::ObjectState_WALK:
			MoveRandom(false);
			return;
		case Define::ObjectState_RUN:
			MoveRandom(true);
			return;
		case Define::ObjectState_SKILL:
			UseSkill();
			break;
		default:
			MoveRandom(false);
			return;
		}
	}

	CORE_TIME_DELEGATE_MANAGER.Push(
		CoreTimeDelegate<>(
			std::bind(&MyPlayer::DoAI, Object::downcasted_shared_from_this<MyPlayer>()),
			CORE_RANDOM_MANAGER_TIME.GetRandom(this->aiMinTime, this->aiMaxTime)));
}

void MyPlayer::MoveRandom(bool isRun)
{
	uint8_t range = static_cast<uint8_t>(CORE_RANDOM_MANAGER_INT.GetRandom(2, 20));
	float_t moveSpeed = 0.0f;
	{
		auto pos = GetPosWithNoLock();

		Define::Dir dir = pos.GetRandomDir();

		auto destPos = pos.GetFrontPos(dir, range);
		if (ZONE_MANAGER.CanMove(this->objectInfoWithPos.pos_info.mapID, destPos))
		{
			SetDirectionWithNoLock(dir);

			this->destDist = range;

			if (isRun)
				SetStateWithNoLock(Define::ObjectState_RUN);
			else
				SetStateWithNoLock(Define::ObjectState_WALK);

			moveSpeed = GetSpeedWithNoLock(Define::SpeedType_MOVE_SPEED);
			moveSpeed = isRun ? moveSpeed * 1.3f : moveSpeed;
		}
	}

	if (0.0f < moveSpeed)
	{
		CORE_TIME_DELEGATE_MANAGER.Push(
			CoreTimeDelegate<>(std::bind(&MyPlayer::Move, Object::downcasted_shared_from_this<MyPlayer>()),
				static_cast<TIME_VALUE>(SEC / CoreUtil::IntRound(moveSpeed))));
	}

	else
	{
		DoAI();
	}
}

void MyPlayer::Move(void)
{
	if (IsDead())
	{
		DoAI();
		return;
	}

	bool isRun = false;
	float_t moveSpeed = 0.0f;
	bool hasNextMove = true;
	{
		auto pos = GetPosWithNoLock();
		auto destPos = pos.GetFrontPos(GetDirWithNoLock());

		if (this->destDist > 1 && ZONE_MANAGER.CanMove(this->objectInfoWithPos.pos_info.mapID, destPos))
		{
			this->destDist -= 1;
			SetPosWithNoLock(destPos);

			moveSpeed = GetSpeedWithNoLock(Define::SpeedType_MOVE_SPEED);

			isRun = GetStateWithNoLock() == Define::ObjectState_RUN ? true : false;

			moveSpeed = isRun ? moveSpeed * 1.3f : moveSpeed;
		}
		else
		{
			hasNextMove = false;

			this->destDist = 0;

			SetPosWithNoLock(pos.GetFrontPos(GetDirWithNoLock()));
			SetStateWithNoLock(Define::ObjectState_IDLE);
		}
	}

	if (hasNextMove)
	{
		GAME_PACKET_SEND_MANAGER.Clear();
		auto packedPosInfo = flatbuffers::PackPositionInfo(GetPosInfo());
		auto message = GamePacket::CreateCS_MOVE_REQ(GAME_PACKET_SEND_MANAGER.builder, isRun, &packedPosInfo);
		GAME_PACKET_SEND_MANAGER.Send(session, GamePacket::Packet_CS_MOVE_REQ, message.Union());

		CORE_TIME_DELEGATE_MANAGER.Push(
			CoreTimeDelegate<>(std::bind(&MyPlayer::Move, Object::downcasted_shared_from_this<MyPlayer>()),
				static_cast<TIME_VALUE>(SEC / CoreUtil::IntRound(moveSpeed))));
	}
	else
	{
		GAME_PACKET_SEND_MANAGER.Clear();
		auto message = GamePacket::CreateCS_SET_STATE_REQ(GAME_PACKET_SEND_MANAGER.builder, Define::ObjectState_IDLE);
		GAME_PACKET_SEND_MANAGER.Send(session, GamePacket::Packet_CS_SET_STATE_REQ, message.Union());

		DoAI();
	}

}
