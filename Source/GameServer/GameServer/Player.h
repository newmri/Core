#pragma once

class Player : public Creature, public CoreMemoryPoolObj<Player, CORE_BIG_SIZE>
{
	OVERRIDE_GAME_OBJ(Player)

public:
	Player(const int64_t& uid, const std::shared_ptr<CoreClientSession> session,
		const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo);

private:
	int64_t uid;
	std::shared_ptr<CoreClientSession> session;
	GamePacket::MyCharacterInfoT characterInfo;
};