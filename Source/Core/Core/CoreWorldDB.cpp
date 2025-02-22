#include "CoreInclude.h"

CoreWorldDB::CoreWorldDB(std::wstring_view dbName, const uint8_t worldID, const uint8_t serverID) : CoreDB(dbName, worldID, serverID)
{

}

CoreWorldDB::~CoreWorldDB()
{

}

void CoreWorldDB::GetServerInfo(ServerInfo& serverInfo)
{
	Prepare(L"GetServerInfo");

	BindArgument(this->worldID);
	BindArgument(this->serverID);

	if (!Execute())
	{
		CORE_ERROR_LOG("Failed");
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		abort();
		return;
	}

	BindCol(&serverInfo.GroupID, sizeof(serverInfo.GroupID));
	BindCol(&serverInfo.ServerPort, sizeof(serverInfo.ServerPort));

	do
	{
		this->retCode = SQLFetch(this->hstmt);

	} while (IsSuccess());

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}