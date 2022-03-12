#include "Include.h"
#include "boost/current_function.hpp"

GameDB::GameDB(std::wstring_view dbName) : CoreDB(dbName)
{

}

GameDB::~GameDB()
{
	Release();
}

void GameDB::Release(void)
{
}

bool GameDB::LoadCharacter(const int64_t accountUID, GamePacket::MyCharacterInfoT& info)
{
	Prepare(L"LoadGameCharacter");
	BindArgument(accountUID);
	BindArgument(info.uid);
	Execute();

	wchar_t name[Define::CharacterLimit_MaxNameLen + 1] = L"";
	BindCol((wchar_t**)&name, sizeof(name));
	BindCol(&info.level, sizeof(info.level));
	BindCol((uint8_t*)&info.job, sizeof(info.job));
	BindCol(&info.exp, sizeof(info.exp));

	for (int i = 0; i < Define::GearType_END; ++i)
		BindCol(&info.gear.index[i], sizeof(info.gear.index[i]));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
		if (IsSuccess())
		{
			SQLFreeStmt(this->hstmt, SQL_CLOSE);

			info.name = STRING_MANAGER.Narrow(name);
			return LoadCharacterStat(accountUID, info);
		}
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
	return false;
}

bool GameDB::LoadCharacterStat(const int64_t accountUID, GamePacket::MyCharacterInfoT& info)
{
	Prepare(L"LoadCharacterStat");
	BindArgument(accountUID);
	BindArgument(info.uid);
	Execute();

	BindCol(&info.hp, sizeof(info.hp));
	BindCol(&info.mp, sizeof(info.mp));

	for (int i = 0; i < Define::StatType_END; ++i)
		BindCol(&info.stat.value[i], sizeof(info.stat.value[i]));

	BindCol(&info.bonus_stat, sizeof(info.bonus_stat));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
		if (IsSuccess())
		{
			SQLFreeStmt(this->hstmt, SQL_CLOSE);
			return true;
		}
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
	return false;
}

uint8_t GameDB::LoadMaxCharacterSlotCount(const int64_t accountUID)
{
	Prepare(L"LoadMaxCharacterSlotCount");
	BindArgument(accountUID);
	Execute();

	uint8_t maxCharacterSlotCount = Define::CharacterLimit_EmptyCharacterSlot;
	BindCol(&maxCharacterSlotCount, sizeof(maxCharacterSlotCount));

	if(IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
		if (IsNoData())
			UpdateMaxCharacterSlotCount(accountUID, maxCharacterSlotCount);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return maxCharacterSlotCount;
}

void GameDB::UpdateMaxCharacterSlotCount(const int64_t accountUID, const uint8_t maxCharacterSlotCount)
{
	Prepare(L"UpdateMaxCharacterSlotCount");
	BindArgument(accountUID);
	BindArgument(maxCharacterSlotCount);
	Execute();

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}