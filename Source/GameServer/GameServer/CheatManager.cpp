#include "Include.h"

IMPLEMENT_SINGLETON(CheatManager)

void CheatManager::Init(void)
{
}

void CheatManager::Release(void)
{
	GetInstance().~CheatManager();
}

void CheatManager::OnCheat(std::string message, std::shared_ptr<Player> player)
{
	// 치트 권한 체크 추가 필요

	message.erase(message.begin());

	std::stringstream ss(message);
	std::string cheatType;
	ss >> cheatType;

	switch (HashCode(cheatType.c_str()))
	{
	case HashCode("item"): // /item ItemID ItemCount
	{
		int32_t itemID = 0;
		uint16_t itemCount = 0;
		ss >> itemID >> itemCount;
		player->AddItemToInventory(itemID, itemCount);
	}
	break;
	case HashCode("level"): // /level level
	{
		uint32_t level = 0;

		ss >> level;
		player->OnLevelUpCheat(static_cast<uint8_t>(level));
	}
	break;
	default:
		break;
	}
}
