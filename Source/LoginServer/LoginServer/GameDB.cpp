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

bool GameDB::CreateCharacter(const int64_t accountUID, const wchar_t* name, const uint8_t level, const Define::Job job, int64_t& uid)
{
	Prepare(L"CreateCharacter");
	BindArgument(accountUID);
	BindArgument(name);
	BindArgument(level);
	BindArgument(job);
	BindArgument(100);
	BindArgument(100);

	for (int i = 0; i <= Define::StatType_MAX; ++i)
		BindArgument(DATA_MANAGER.characterCreateStat[job].Stat[i]);

	for (int i = 0; i <= Define::GearType_MAX; ++i)
		BindArgument(DATA_MANAGER.characterCreateGear[job].Gear[i]);

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
			uid = characterUID;
		}
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return isSuccess;
}