#include "CoreInclude.h"

CoreGameDB::CoreGameDB(std::wstring_view dbName) : CoreDB(dbName)
{

}

CoreGameDB::~CoreGameDB()
{

}

CoreItemUID CoreGameDB::GetItemUID(const uint8_t worldID, const uint8_t serverID, const uint16_t count)
{
	Prepare(L"GetItemUID");
	BindArgument(serverID);
	BindArgument(count);
	Execute();

	CoreItemUID itemUID;
	BindCol(&itemUID.uid, sizeof(itemUID.uid));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	itemUID.worldID = worldID;
	itemUID.serverID = serverID;
	return itemUID;
}
