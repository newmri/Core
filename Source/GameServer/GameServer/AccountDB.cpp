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

bool AccountDB::Login(const int64_t accountUID, CoreToken& token)
{
	Prepare(L"Login");
	BindArgument(accountUID);
	BindArgument(token.key);
	Execute();

	bool isSuccess = false;
	int32_t time = 0;

	BindCol(&isSuccess, sizeof(isSuccess));
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

	return isSuccess;
}

void AccountDB::Logout(const int64_t accountUID)
{
	Prepare(L"Logout");
	BindArgument(accountUID);
	Execute();

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}

void AccountDB::LoadMoney(const int64_t accountUID, NativeInfo::Money& money)
{
	Prepare(L"LoadMoney");
	BindArgument(accountUID);
	Execute();

	for (int32_t i = 0; i < Define::MoneyType_END; ++i)
		BindCol(&money.value[i], sizeof(money.value[i]));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}