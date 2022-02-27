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
	BindArgument(DATA_MANAGER.createStat[job].STR);
	BindArgument(DATA_MANAGER.createStat[job].DEX);
	BindArgument(DATA_MANAGER.createStat[job].INT);
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