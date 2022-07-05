#include "Include.h"
#include "boost/current_function.hpp"

GameDB::GameDB(std::wstring_view dbName) : CoreGameDB(dbName)
{

}

GameDB::~GameDB()
{

}

bool GameDB::LoadCharacter(const int64_t accountUID, const int64_t uid, Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo)
{
	Prepare(L"LoadGameCharacter");
	BindArgument(accountUID);
	BindArgument(uid);
	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, "accountUID: " + TO_STR(accountUID) + " uid: " + TO_STR(uid) + " ", __FILE__, __FUNCTION__, __LINE__));
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return false;
	}

	wchar_t name[Define::CharacterLimit_MaxNameLen + 1] = L"";
	BindCol((wchar_t**)&name, sizeof(name));
	BindCol(&creatureInfo.level, sizeof(creatureInfo.level));
	BindCol((uint8_t*)&characterInfo.job, sizeof(characterInfo.job));
	BindCol(&creatureInfo.exp, sizeof(creatureInfo.exp));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
		if (IsSuccess())
		{
			SQLFreeStmt(this->hstmt, SQL_CLOSE);

			characterInfo.name = STRING_MANAGER.Narrow(name);
			if (!LoadCharacterStat(accountUID, uid, creatureInfo, characterInfo))
				return false;

			return LoadCharacterGear(accountUID, uid, characterInfo);
		}
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
	return false;
}

bool GameDB::LoadCharacterStat(const int64_t accountUID, const int64_t uid, Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo)
{
	Prepare(L"LoadCharacterStat");
	BindArgument(accountUID);
	BindArgument(uid);
	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, "accountUID: " + TO_STR(accountUID) + " uid: " + TO_STR(uid) + " ", __FILE__, __FUNCTION__, __LINE__));
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return false;
	}

	BindCol(&creatureInfo.hp, sizeof(creatureInfo.hp));
	BindCol(&creatureInfo.mp, sizeof(creatureInfo.mp));

	for (int i = 0; i < Define::StatType_END; ++i)
		BindCol(&creatureInfo.stat.value[i], sizeof(creatureInfo.stat.value[i]));

	BindCol(&characterInfo.bonus_stat, sizeof(characterInfo.bonus_stat));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
		if (IsSuccess())
		{
			SQLFreeStmt(this->hstmt, SQL_CLOSE);
			return true;
		}
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
	return false;
}

bool GameDB::LoadCharacterGear(const int64_t accountUID, const int64_t uid, GamePacket::MyCharacterInfoT& characterInfo)
{
	Prepare(L"LoadCharacterGear");
	BindArgument(accountUID);
	BindArgument(uid);

	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, "accountUID: " + TO_STR(accountUID) + " uid: " + TO_STR(uid) + " ", __FILE__, __FUNCTION__, __LINE__));
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return false;
	}

	NativeInfo::GearSlotInfo gear;
	BindCol(&gear.itemUID, sizeof(gear.itemUID));
	BindCol(&gear.itemID, sizeof(gear.itemID));

	int32_t count = 0;
	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);

		if (IsSuccess())
		{
			characterInfo.gear.info[count++] = gear;
		}
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
	return true;
}

uint8_t GameDB::LoadMaxCharacterSlotCount(const int64_t accountUID)
{
	Prepare(L"LoadMaxCharacterSlotCount");
	BindArgument(accountUID);
	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, "accountUID: " + TO_STR(accountUID) + " ", __FILE__, __FUNCTION__, __LINE__));
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return 0;
	}

	uint8_t maxCharacterSlotCount = Define::CharacterLimit_EmptyCharacterSlot;
	BindCol(&maxCharacterSlotCount, sizeof(maxCharacterSlotCount));

	if(IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
		if (IsNoData())
			UpdateMaxCharacterSlotCount(accountUID, maxCharacterSlotCount);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
	return maxCharacterSlotCount;
}

void GameDB::UpdateMaxCharacterSlotCount(const int64_t accountUID, const uint8_t maxCharacterSlotCount)
{
	Prepare(L"UpdateMaxCharacterSlotCount");
	BindArgument(accountUID);
	BindArgument(maxCharacterSlotCount);
	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, "accountUID: " + TO_STR(accountUID) + " ", __FILE__, __FUNCTION__, __LINE__));
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return;
	}

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}

uint8_t GameDB::LoadMaxItemInventorySlotCount(const int64_t accountUID, const int64_t uid)
{
	Prepare(L"LoadMaxItemInventorySlotCount");
	BindArgument(accountUID);
	BindArgument(uid);
	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, "accountUID: " + TO_STR(accountUID) + " uid: " + TO_STR(uid) + " ", __FILE__, __FUNCTION__, __LINE__));
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return 0;
	}

	uint8_t maxItemInventorySlotCount = 0;
	BindCol(&maxItemInventorySlotCount, sizeof(maxItemInventorySlotCount));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
	return maxItemInventorySlotCount;
}

bool GameDB::LoadItemInventory(const int64_t accountUID, const int64_t uid, std::unordered_map<int64_t, Info::ItemSlotInfoT>& itemInventory)
{
	Prepare(L"LoadItemInventory");
	BindArgument(accountUID);
	BindArgument(uid);
	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, "accountUID: " + TO_STR(accountUID) + " uid: " + TO_STR(uid) + " ", __FILE__, __FUNCTION__, __LINE__));
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return false;
	}

	Info::ItemSlotInfoT itemSlotInfo;

	BindCol(&itemSlotInfo.item_uid, sizeof(itemSlotInfo.item_uid));
	BindCol(&itemSlotInfo.item_id, sizeof(itemSlotInfo.item_id));
	BindCol(&itemSlotInfo.item_count, sizeof(itemSlotInfo.item_count));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
		if (IsSuccess())
		{
			itemInventory[itemSlotInfo.item_uid] = itemSlotInfo;
		}
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
	return true;
}

bool GameDB::EquipGear(const int64_t accountUID, const int64_t uid, const Define::GearType gearType, const Info::ItemSlotInfoT& itemSlotInfo)
{
	Prepare(L"EquipGear");
	BindArgument(accountUID);
	BindArgument(uid);
	BindArgument(gearType);
	BindArgument(itemSlotInfo.item_uid);
	BindArgument(itemSlotInfo.item_id);

	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, "accountUID: " + TO_STR(accountUID) + " uid: " + TO_STR(uid) + " ", __FILE__, __FUNCTION__, __LINE__));
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

bool GameDB::UnEquipGear(const int64_t accountUID, const int64_t uid, const Define::GearType gearType, const NativeInfo::GearSlotInfo& gearSlotInfo)
{
	Prepare(L"UnEquipGear");
	BindArgument(accountUID);
	BindArgument(uid);
	BindArgument(gearType);
	BindArgument(gearSlotInfo.itemUID);
	BindArgument(gearSlotInfo.itemID);

	if (!Execute())
	{
		CORE_LOG.Log(CORE_LOG.MakeLog(LogType::LOG_ERROR, "accountUID: " + TO_STR(accountUID) + " uid: " + TO_STR(uid) + " ", __FILE__, __FUNCTION__, __LINE__));
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
