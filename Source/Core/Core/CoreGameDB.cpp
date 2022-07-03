#include "CoreInclude.h"

CoreGameDB::CoreGameDB(std::wstring_view dbName) : CoreDB(dbName)
{

}

CoreGameDB::~CoreGameDB()
{

}

CoreItemUID CoreGameDB::GetItemUID(const uint8_t worldID, const uint8_t serverID, const uint16_t count)
{
	CoreItemUID itemUID;

	Prepare(L"GetItemUID");
	BindArgument(serverID);
	BindArgument(count);
	if (!Execute())
	{
		CORE_LOG.Log(LogType::LOG_ERROR, "worldID: " + TO_STR(worldID) + "serverID: " + TO_STR(serverID));
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return itemUID;
	}

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
