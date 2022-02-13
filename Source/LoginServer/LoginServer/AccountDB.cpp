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

int AccountDB::Login(const wchar_t* ID, const wchar_t* password)
{
	Prepare(L"Login");
	BindArgument(ID);
	BindArgument(password);
	Execute();

	int result = 0;

	BindCol(&result, sizeof(result));

	do
	{
		this->retCode = SQLFetch(this->hstmt);

	} while (IsSuccess());

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return result;
}