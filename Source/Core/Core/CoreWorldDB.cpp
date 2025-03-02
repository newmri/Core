#include "CoreInclude.h"

CoreWorldDB::CoreWorldDB(DBInfo&& dbInfo) : CoreDB(std::move(dbInfo))
{

}

CoreWorldDB::~CoreWorldDB()
{

}

void CoreWorldDB::GetServerInfo(ServerInfo& serverInfo)
{
	Prepare(L"GetServerInfo");

	BindArgument(this->dbInfo.worldID);
	BindArgument(this->dbInfo.serverID);

	if (!Execute())
	{
		CORE_ERROR_LOG("Failed");
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		abort();
		return;
	}

	BindCol(&serverInfo.groupID, sizeof(serverInfo.groupID));
	BindCol(&serverInfo.serverPort, sizeof(serverInfo.serverPort));

	do
	{
		this->retCode = SQLFetch(this->hstmt);

	} while (IsSuccess());

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}