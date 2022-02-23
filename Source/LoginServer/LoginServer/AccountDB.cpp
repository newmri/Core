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

bool AccountDB::Login(const int64_t uid, const int32_t token, int32_t& expireTime)
{
	Prepare(L"Login");
	BindArgument(uid);
	BindArgument(token);
	Execute();

	bool result = 0;

	BindCol(&result, sizeof(result));
	BindCol(&expireTime, sizeof(expireTime));

	do
	{
		this->retCode = SQLFetch(this->hstmt);

	} while (IsSuccess());

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return result;
}