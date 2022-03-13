#pragma once

// Player의 uid는 oid라고 볼 수 있음
class Player : public CoreUIDObj, public CoreMemoryPoolObj<Player, CORE_BIG_SIZE>, public std::enable_shared_from_this<Player>
{
	OVERRIDE_OBJ_DELETE_DEFAULT_CONSTRUCTOR(Player)

public:
	Player(const int64_t& uid, const std::shared_ptr<CoreClientSession> session, const std::shared_ptr<Character> character);

private:
	std::shared_ptr<CoreClientSession> session;
	std::shared_ptr<Character> character;
};