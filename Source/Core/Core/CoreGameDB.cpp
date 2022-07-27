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
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, "count: " + TO_STR(count) + " ", __FILE__, __FUNCTION__, __LINE__));
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

void CoreGameDB::AddItemToInventory(const ItemCreateSlotInfo& itemCreateSlotInfo, std::vector<Info::ItemSlotInfoT>& itemSlotInfoList)
{
	auto itemUID = GetItemUID(itemCreateSlotInfo.needSlotCount);
	if (IS_ZERO(itemUID.uid))
		return;

	bool isSuccess = false;
	uint16_t addItemCount = 0;
	uint16_t remainedItemCount = itemCreateSlotInfo.itemCount;
	Info::ItemSlotInfoT itemSlotInfo;
	for (uint8_t i = 0; i < itemCreateSlotInfo.needSlotCount; ++i)
	{
		addItemCount = itemCreateSlotInfo.itemCount / itemCreateSlotInfo.maxStackCount;
		if (remainedItemCount < addItemCount)
			addItemCount = remainedItemCount;

		Prepare(L"AddItemToInventory");
		BindArgument(itemCreateSlotInfo.accountUID);
		BindArgument(itemCreateSlotInfo.uid);
		BindArgument(itemUID.uid);
		BindArgument(itemCreateSlotInfo.itemID);
		BindArgument(addItemCount);

		if (!Execute())
		{
			CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, 
				"accountUID: " + TO_STR(itemCreateSlotInfo.accountUID) +
				"UID: " + TO_STR(itemCreateSlotInfo.uid) +
				"itemID: " + TO_STR(itemCreateSlotInfo.itemID) +
				"itemCount: " + TO_STR(itemCreateSlotInfo.itemCount) +
				" ",
				__FILE__, __FUNCTION__, __LINE__));

			SQLFreeStmt(this->hstmt, SQL_CLOSE);
			return;
		}

		BindCol(&isSuccess, sizeof(isSuccess));

		while (IsSuccess())
		{
			this->retCode = SQLFetch(this->hstmt);
		};

		SQLFreeStmt(this->hstmt, SQL_CLOSE);

		if (isSuccess)
		{
			itemSlotInfo.item_uid = itemUID.uid;
			itemSlotInfo.item_id = itemCreateSlotInfo.itemID;
			itemSlotInfo.item_count = addItemCount;

			itemSlotInfoList.push_back(itemSlotInfo);
		}
		else
		{
			CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR,
				"accountUID: " + TO_STR(itemCreateSlotInfo.accountUID) +
				"UID: " + TO_STR(itemCreateSlotInfo.uid) +
				"itemID: " + TO_STR(itemCreateSlotInfo.itemID) +
				"itemCount: " + TO_STR(addItemCount) +
				" ",
				__FILE__, __FUNCTION__, __LINE__));
		}

		++itemUID.uid;
		remainedItemCount -= addItemCount;
	}
}
