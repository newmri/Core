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
		addItemCount = itemCreateSlotInfo.maxStackCount;
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

bool CoreGameDB::UpdateInventoryItemCount(const int64_t accountUID, const int64_t uid, Info::ItemSlotInfoT* itemSlotInfo, const uint16_t useCount)
{
	Prepare(L"UpdateInventoryItemCount");
	BindArgument(accountUID);
	BindArgument(uid);
	BindArgument(itemSlotInfo->item_uid);
	BindArgument(useCount);

	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR,
			"accountUID: " + TO_STR(accountUID) +
			"UID: " + TO_STR(uid) +
			"item_uid: " + TO_STR(itemSlotInfo->item_uid) +
			"useCount: " + TO_STR(useCount) +
			" ",
			__FILE__, __FUNCTION__, __LINE__));

		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return false;
	}

	bool isSuccess = false;
	BindCol(&isSuccess, sizeof(isSuccess));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	if (isSuccess)
	{
		itemSlotInfo->item_count -= useCount;
	}

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
	return isSuccess;
}

bool CoreGameDB::DeleteInventoryItem(const int64_t accountUID, const int64_t uid, Info::ItemSlotInfoT* itemSlotInfo)
{
	Prepare(L"DeleteInventoryItem");
	BindArgument(accountUID);
	BindArgument(uid);
	BindArgument(itemSlotInfo->item_uid);

	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR,
			"accountUID: " + TO_STR(accountUID) +
			"UID: " + TO_STR(uid) +
			"item_uid: " + TO_STR(itemSlotInfo->item_uid) +
			" ",
			__FILE__, __FUNCTION__, __LINE__));

		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return false;
	}

	bool isSuccess = false;
	BindCol(&isSuccess, sizeof(isSuccess));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	if (isSuccess)
	{
		memset(itemSlotInfo, 0, sizeof(Info::ItemSlotInfoT));
	}

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
	return isSuccess;;
}

bool CoreGameDB::OnLevelUp(const int64_t accountUID, const int64_t uid, const uint8_t newLevel, const int32_t newBonusStatPoint, const int32_t newHP, const int32_t newMP)
{
	CoreItemUID itemUID;

	Prepare(L"LevelUp");
	BindArgument(accountUID);
	BindArgument(uid);
	BindArgument(newLevel);
	BindArgument(newBonusStatPoint);
	BindArgument(newHP);
	BindArgument(newMP);

	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR,
			"accountUID: " + TO_STR(accountUID) +
			"UID: " + TO_STR(uid) +
			"newLevel: " + TO_STR(newLevel) +
			"newBonusStatPoint: " + TO_STR(newBonusStatPoint) +
			" ",
			__FILE__, __FUNCTION__, __LINE__));

		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return false;
	}

	bool isSuccess = false;
	BindCol(&isSuccess, sizeof(isSuccess));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
	return isSuccess;
}