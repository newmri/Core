#include "CoreInclude.h"

CoreGameDB::CoreGameDB(DBInfo&& dbInfo) : CoreDB(std::move(dbInfo))
{
}

CoreGameDB::~CoreGameDB()
{

}

void CoreGameDB::AddItemToInventory(const ItemCreateSlotInfo& itemCreateSlotInfo, std::vector<Info::ItemSlotInfoT>& itemSlotInfoList)
{
	int64_t itemUID = 0;

	bool isSuccess = false;
	uint16_t addItemCount = 0;
	uint16_t remainedItemCount = itemCreateSlotInfo.itemCount;
	Info::ItemSlotInfoT itemSlotInfo;
	for (uint8_t i = 0; i < itemCreateSlotInfo.needSlotCount; ++i)
	{
		itemUID = CORE_UID_GENERATOR.GetUID(this->dbInfo);

		addItemCount = itemCreateSlotInfo.maxStackCount;
		if (remainedItemCount < addItemCount)
			addItemCount = remainedItemCount;

		Prepare(L"AddItemToInventory");
		BindArgument(itemCreateSlotInfo.accountUID);
		BindArgument(itemCreateSlotInfo.uid);
		BindArgument(itemUID);
		BindArgument(itemCreateSlotInfo.itemID);
		BindArgument(addItemCount);

		if (!Execute())
		{
			CORE_ERROR_LOG(
				"accountUID: {} UID: {} itemUID: {} itemID: {} itemCount: {}"
				, itemCreateSlotInfo.accountUID
				, itemCreateSlotInfo.uid
				, itemUID
				, itemCreateSlotInfo.itemID
				, itemCreateSlotInfo.itemCount
				);

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
			itemSlotInfo.item_uid = itemUID;
			itemSlotInfo.item_id = itemCreateSlotInfo.itemID;
			itemSlotInfo.item_count = addItemCount;

			itemSlotInfoList.push_back(itemSlotInfo);
		}
		else
		{
			CORE_ERROR_LOG(
				"accountUID: {} UID: {} itemUID: {} itemID: {} itemCount: {}"
				, itemCreateSlotInfo.accountUID
				, itemCreateSlotInfo.uid
				, itemUID
				, itemCreateSlotInfo.itemID
				, itemCreateSlotInfo.itemCount
			);
		}

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
		CORE_ERROR_LOG(
			"accountUID: {} UID: {} item_uid: {} useCount: {}"
			, accountUID
			, uid
			, itemSlotInfo->item_uid
			, useCount
		);

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
		CORE_ERROR_LOG(
			"accountUID: {} UID: {} item_uid: {} useCount: {}"
			, accountUID
			, uid
			, itemSlotInfo->item_uid
		);

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
		CORE_ERROR_LOG(
			"accountUID: {} UID: {} item_uid: {} newLevel: {} newBonusStatPoint: {}"
			, accountUID
			, uid
			, newLevel
			, newBonusStatPoint
		);

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