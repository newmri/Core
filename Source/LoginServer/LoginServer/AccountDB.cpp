#include "Include.h"
#include "boost/current_function.hpp"

AccountDB::AccountDB(std::wstring_view dbName) : CoreDB(dbName)
{
	CORE_ACCOUNT_MANAGER.SetLogoutFunc(std::bind(&AccountDB::Logout, this, std::placeholders::_1));
}

AccountDB::~AccountDB()
{
	Release();
}

void AccountDB::Release(void)
{
}

bool AccountDB::Login(const int64_t uid, CoreToken& token)
{
	Prepare(L"Login");
	BindArgument(uid);
	BindArgument(token.key);
	Execute();

	bool result = 0;
	int32_t time = 0;

	BindCol(&result, sizeof(result));
	BindCol(&time, sizeof(time));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);

		if (IsSuccess())
		{
			token.expireTime = time;
		}
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return result;
}

void AccountDB::Logout(const int64_t uid)
{
	Prepare(L"Logout");
	BindArgument(uid);
	Execute();

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}