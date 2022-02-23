#include "Include.h"
#include "boost/current_function.hpp"

AccountDB::AccountDB(std::wstring_view dbName) : CoreDB(dbName)
{

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

	do
	{
		this->retCode = SQLFetch(this->hstmt);
		token.expireTime = time;

	} while (IsSuccess());

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return result;
}