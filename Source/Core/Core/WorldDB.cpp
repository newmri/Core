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

void WorldDB::GerServerInfo(const int worldID, const int serverID, ServerInfo& serverInfo)
{
	Prepare(L"GetServerInfo");

	BindArgument(worldID);
	BindArgument(serverID);

	Execute();

	BindCol(&serverInfo.GroupID, sizeof(serverInfo.GroupID));
	BindCol(&serverInfo.ServerPort, sizeof(serverInfo.ServerPort));

	do
	{
		this->retCode = SQLFetch(this->hstmt);

	} while (IsSuccess());

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}