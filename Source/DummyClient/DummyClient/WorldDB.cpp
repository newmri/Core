#include "Include.h"
#include "boost/current_function.hpp"

WorldDB::WorldDB(std::wstring_view dbName) : CoreDB(dbName)
{
	
}

WorldDB::~WorldDB()
{
	Release();
}

void WorldDB::WorldDB(void)
{
}

void WorldDB::IncreaseUserCount(const int64_t accountUID)
{
	Prepare(L"IncreaseUserCount");
	BindArgument(accountUID);
	BindArgument(LOGIN_SERVER.GetWorldID());
	BindArgument(LOGIN_SERVER.GetServerID());
	Execute();

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}
