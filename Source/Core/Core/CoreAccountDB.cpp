#include "CoreInclude.h"

CoreAccountDB::CoreAccountDB(std::wstring_view dbName, const uint8_t worldID, const uint8_t serverID) : CoreDB(dbName, worldID, serverID)
{

}

CoreAccountDB::~CoreAccountDB()
{

}

bool CoreAccountDB::Login(const int64_t accountUID, CoreToken& token)
{
	Prepare(L"Login");
	BindArgument(accountUID);
	BindArgument(token.key);
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