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

void GameDB::LoadCharacter(const int64_t accountUID, std::vector<CharacterLoadInfo>& infoList)
{
	Prepare(L"LoadLoginCharacter");
	BindArgument(accountUID);
	Execute();

	CharacterLoadInfo info;
	BindCol(&info.uid, sizeof(info.uid));
	BindCol((wchar_t**)&info.name, Define::CharacterLimit_MaxNameLen);
	BindCol(&info.info.level, sizeof(info.info.level));
	BindCol(&info.info.job, sizeof(info.info.job));

	for (int i = 0; i <= Define::GearType_MAX; ++i)
		BindCol(&info.info.gear.value[i], sizeof(info.info.gear.value[i]));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);

		if (IsSuccess())
		{
			infoList.push_back(info);
		}
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}

bool GameDB::CreateCharacter(const int64_t accountUID, CharacterLoadInfo& loadInfo)
{
	Prepare(L"CreateCharacter");
	BindArgument(accountUID);
	BindArgument(loadInfo.name);
	BindArgument(loadInfo.info.level);
	BindArgument(loadInfo.info.job);
	BindArgument(100);
	BindArgument(100);

	for (int i = 0; i <= Define::StatType_MAX; ++i)
		BindArgument(loadInfo.info.stat.value[i]);

	for (int i = 0; i <= Define::GearType_MAX; ++i)
		BindArgument(loadInfo.info.gear.value[i]);

	Execute();

	bool isSuccess = false;
	int64_t characterUID = 0;

	BindCol(&isSuccess, sizeof(isSuccess));
	BindCol(&characterUID, sizeof(characterUID));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);

		if (IsSuccess())
		{
			loadInfo.uid = characterUID;
		}
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return isSuccess;
}