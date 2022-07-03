#include "Include.h"
#include "boost/current_function.hpp"

AccountDB::AccountDB(std::wstring_view dbName) : CoreAccountDB(dbName)
{
	CORE_ACCOUNT_MANAGER.SetLogoutFunc(std::bind(&AccountDB::Logout, this, std::placeholders::_1));
}

AccountDB::~AccountDB()
{

}

void AccountDB::Logout(const CoreAccount* account)
{
	Prepare(L"Logout");
	BindArgument(account->GetUID());
	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, "accountUID: " + TO_STR(account->GetUID()) + " ", __FILE__, __FUNCTION__, __LINE__));
		return;
	}

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	OBJECT_MANAGER.RemovePlayer(account->GetPlayerOID());
}

void AccountDB::LoadMoney(const int64_t accountUID, NativeInfo::Money& money)
{
	Prepare(L"LoadMoney");
	BindArgument(accountUID);
	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, "accountUID: " + TO_STR(accountUID) + " ", __FILE__, __FUNCTION__, __LINE__));
		return;
	}

	for (int32_t i = 0; i < Define::MoneyType_END; ++i)
		BindCol(&money.value[i], sizeof(money.value[i]));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}