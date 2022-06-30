#include "CoreInclude.h"

CoreWorldDB::CoreWorldDB(std::wstring_view dbName) : CoreDB(dbName)
{

}

CoreWorldDB::~CoreWorldDB()
{

}

void CoreWorldDB::SetID(const uint8_t worldID, const uint8_t serverID)
{
	this->worldID = worldID;
	this->serverID = serverID;
}

void CoreWorldDB::GetServerInfo(ServerInfo& serverInfo)
{
	Prepare(L"GetServerInfo");

	BindArgument(this->worldID);
	BindArgument(this->serverID);

	Execute();

	BindCol(&serverInfo.GroupID, sizeof(serverInfo.GroupID));
	BindCol(&serverInfo.ServerPort, sizeof(serverInfo.ServerPort));

	do
	{
		this->retCode = SQLFetch(this->hstmt);

	} while (IsSuccess());

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}

void CoreWorldDB::IncreaseUserCount(void)
{
	Prepare(L"IncreaseUserCount");

	BindArgument(this->worldID);
	BindArgument(this->serverID);

	Execute();

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}