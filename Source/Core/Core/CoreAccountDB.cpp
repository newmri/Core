#include "CoreInclude.h"

CoreAccountDB::CoreAccountDB(DBInfo&& dbInfo) : CoreDB(std::move(dbInfo))
{
}

CoreAccountDB::~CoreAccountDB()
{

}

void CoreAccountDB::Init(void)
{
	CoreDB::Init();

	Prepare(L"ResetLogin");
	BindArgument(this->dbInfo.worldID);
	BindArgument(this->dbInfo.serverID);
	if (!Execute())
	{
		CORE_ERROR_LOG("ResetLogin Fail");
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
	BindArgument(this->dbInfo.worldID);
	BindArgument(this->dbInfo.serverID);
	if (!Execute())
	{
		CORE_ERROR_LOG("accountUID: {}", accountUID);
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
	BindArgument(this->dbInfo.worldID);
	BindArgument(this->dbInfo.serverID);
	if (!Execute())
	{
		CORE_ERROR_LOG("accountUID: {}", accountUID);
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return;
	}

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}

void CoreAccountDB::Logout(std::shared_ptr<CoreAccount> account)
{
	Prepare(L"Logout");
	BindArgument(account->GetUID());
	BindArgument(this->dbInfo.worldID);
	BindArgument(this->dbInfo.serverID);
	if (!Execute())
	{
		CORE_ERROR_LOG("accountUID: {}",account->GetUID());
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return;
	}

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}

