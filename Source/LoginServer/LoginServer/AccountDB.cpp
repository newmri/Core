#include "Include.h"
#include "boost/current_function.hpp"

AccountDB::AccountDB(std::wstring_view dbName, const uint8_t worldID, const uint8_t serverID) : CoreAccountDB(dbName, worldID, serverID)
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
	BindArgument(LOGIN_SERVER.GetWorldID());
	BindArgument(LOGIN_SERVER.GetServerID());
	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, "accountUID: " + TO_STR(account->GetUID()) + " ", __FILE__, __FUNCTION__, __LINE__));
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return;
	}

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}
