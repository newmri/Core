#pragma once

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
	virtual void MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet) override;

public:
	void Send(GamePacket::Packet packetType, flatbuffers::Offset<void> packet);

public:
	virtual void SetState(const Define::ObjectState state) override;

public:
	virtual void AddSkill(const int32_t skillID) override;

private:
	std::shared_ptr<CoreServerSession> session;
	GamePacket::MyCharacterInfoT characterInfo;

private:
	TIME_VALUE lastMoveTime = 0;
};