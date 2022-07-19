#pragma once

#define CHEAT_MANAGER GET_INSTANCE(CheatManager)

class CheatManager
{
	DECLARE_SINGLETON(CheatManager)

public:
	void OnCheat(std::wstring message, std::shared_ptr<Player> player);
};