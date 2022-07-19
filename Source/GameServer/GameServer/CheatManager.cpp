#include "Include.h"

IMPLEMENT_SINGLETON(CheatManager)

void CheatManager::Init(void)
{
}

void CheatManager::Release(void)
{
	GetInstance().~CheatManager();
}

void CheatManager::OnCheat(std::wstring message, std::shared_ptr<Player> player)
{
	// 치트 권한 체크 추가 필요

	message.erase(message.begin());

	std::wstringstream ss(message);
	std::wstring cheatType;
	ss >> cheatType;

	// /item ItemID ItemCount
	if (IS_SAME(L"item", cheatType))
	{
		int32_t itemID = 0;
		uint16_t itemCount = 0;
		ss >> itemID >> itemCount;
		player->AddItemToInventory(itemID, itemCount);
	}
}
