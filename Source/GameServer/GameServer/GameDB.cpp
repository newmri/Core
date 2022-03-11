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

bool GameDB::LoadCharacter(const int64_t accountUID, const int64_t characterUID, CharacterLoadInfo& info)
{
	Prepare(L"LoadGameCharacter");
	BindArgument(accountUID);
	BindArgument(characterUID);
	Execute();

	BindCol((wchar_t**)&info.name, Define::CharacterLimit_MaxNameLen);
	BindCol(&info.info.level, sizeof(info.info.level));
	BindCol(&info.info.job, sizeof(info.info.job));

	for (int i = 0; i < Define::GearType_GEAR_END; ++i)
		BindCol(&info.info.gear.value[i], sizeof(info.info.gear.value[i]));

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