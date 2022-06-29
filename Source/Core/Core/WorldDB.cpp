#include "CoreInclude.h"

WorldDB::WorldDB(std::wstring_view dbName) : CoreDB(dbName)
{

}

WorldDB::~WorldDB()
{
	Release();
}

void WorldDB::Release(void)
{
}

void WorldDB::SetID(const int16_t worldID, const int16_t serverID)
{
	this->worldID = worldID;
	this->serverID = serverID;
}

void WorldDB::GetServerInfo(ServerInfo& serverInfo)
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

void WorldDB::IncreaseUserCount(void)
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