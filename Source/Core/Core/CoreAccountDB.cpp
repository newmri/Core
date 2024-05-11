#include "CoreInclude.h"

CoreAccountDB::CoreAccountDB(std::wstring_view dbName, const uint8_t worldID, const uint8_t serverID) : CoreDB(dbName, worldID, serverID)
{

}

CoreAccountDB::~CoreAccountDB()
{

}

void CoreAccountDB::Init(void)
{
	CoreDB::Init();

	Prepare(L"ResetLogin");
	BindArgument(this->worldID);
	BindArgument(this->serverID);
	if (!Execute())
	{
		CORE_LOG.Log(LogType::LOG_ERROR, "ResetLogin Fail");
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		abort();
	}

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}

bool CoreAccountDB::Login(const int64_t accountUID, CoreToken& token)
{
	Prepare(L"Login");
	BindArgument(accountUID);
	BindArgument(token.key);
	BindArgument(this->worldID);
	BindArgument(this->serverID);
	if (!Execute())
	{
		CORE_LOG.Log(LogType::LOG_ERROR, "accountUID: " + TO_STR(accountUID));
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return false;
	}

	bool isSuccess = false;
	TIME_VALUE time = 0;

	BindCol(&isSuccess, sizeof(isSuccess));
	BindCol(&time, sizeof(time));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);

		if (IsSuccess())
			token.expireTime = time;
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return isSuccess;
}

void CoreAccountDB::SetLogin(const int64_t accountUID)
{
	Prepare(L"SetLogin");
	BindArgument(accountUID);
	BindArgument(this->worldID);
	BindArgument(this->serverID);
	if (!Execute())
	{
		CORE_LOG.Log(LogType::LOG_ERROR, "accountUID: " + TO_STR(accountUID));
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return;
	}

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}

void CoreAccountDB::Logout(std::shared_ptr<CoreAccount> account)
{
	Prepare(L"Logout");
	BindArgument(account->GetUID());
	BindArgument(this->worldID);
	BindArgument(this->serverID);
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

