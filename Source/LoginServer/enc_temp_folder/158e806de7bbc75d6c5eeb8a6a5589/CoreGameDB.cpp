#include "CoreInclude.h"

CoreGameDB::CoreGameDB(std::wstring_view dbName, const uint8_t worldID, const uint8_t serverID) : CoreDB(dbName, worldID, serverID)
{

}

CoreGameDB::~CoreGameDB()
{

}

CoreItemUID CoreGameDB::GetItemUID(const uint16_t count)
{
	CoreItemUID itemUID;

	Prepare(L"GetItemUID");
	BindArgument(this->serverID);
	BindArgument(count);
	if (!Execute())
	{
		CORE_LOG.Log(LogType::LOG_ERROR, "worldID: " + TO_STR(this->worldID) + "serverID: " + TO_STR(this->serverID));
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return itemUID;
	}

	BindCol(&itemUID.uid, sizeof(itemUID.uid));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	itemUID.worldID = this->worldID;
	itemUID.serverID = this->serverID;
	return itemUID;
}

bool CoreGameDB::AddItemToInventory(const int32_t itemID, const uint16_t itemCount, const uint8_t needSlotCount, std::vector<Info::ItemSlotInfoT>& itemSlotInfoList)
{
	if(IS_ZERO(GetItemUID(needSlotCount).uid))
		return false;


}
