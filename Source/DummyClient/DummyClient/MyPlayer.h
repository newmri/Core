#pragma once

enum AIBehavior : int32_t
{
	IDLE,
	WALK,
	RUN,
	SKILL,
	CHAT,
	LEVEL_UP,
	MAX
};

class MyPlayer : public Player, public CoreMemoryPoolObj<MyPlayer, CORE_BIG_SIZE>
{
	OVERRIDE_GAME_OBJECT(MyPlayer)

public:
	MyPlayer(const std::shared_ptr<CoreServerSession> session,
		const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo);

public:
	std::shared_ptr<CoreServerSession> GetSession(void);

public:
	virtual GamePacket::CharacterInfoT GetCharacterInfo(void) override;

public:
	void Send(GamePacket::Packet packetType, flatbuffers::Offset<void> packet);

public:
	virtual void SetState(const Define::ObjectState state) override;

public:
	void AddSkill(const int32_t skillID);
	void UseSkill(void);

public:
	virtual bool OnGetDamage(const GamePacket::DamageInfoT& damageInfo) override;
	void SendReviveReq(void);
	virtual bool Revive(const Info::ObjectInfoWithPosT& objectInfoWithPos) override;
	void OnLevelUp(const uint8_t newLevel, const int32_t newStatPoint);

private:
	void DoAI(void);
	void SetBehavior(const AIBehavior behavior);
	void MoveRandom(bool isRun);
	void Move(void);
	void Chat(std::string_view chatMessage);

private:
	std::shared_ptr<CoreServerSession> session;
	GamePacket::MyCharacterInfoT characterInfo;

private:
	AIBehavior currBehavior = AIBehavior::MAX;
	TIME_VALUE lastMoveTime = 0;
	TIME_VALUE reviveTime = (Define::ReviveTime_COOL_TIME + 1) * SEC;
	TIME_VALUE aiMinTime = 500;
	TIME_VALUE aiMaxTime = 1000;

private:
	uint8_t destDist = 0;
};