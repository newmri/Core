#include "Include.h"
#include "boost/current_function.hpp"

GameDB::GameDB(DBInfo&& dbInfo) : CoreGameDB(std::move(dbInfo))
{
}

GameDB::~GameDB()
{

}

bool GameDB::LoadCharacter(const int64_t accountUID, std::vector<LoginPacket::CharacterInfoT>& infoList)
{
	Prepare(L"LoadLoginCharacter");
	BindArgument(accountUID);
	if (!Execute())
	{
		CORE_ERROR_LOG("accountUID: {}", accountUID);
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return false;
	}

	LoginPacket::CharacterInfoT info;
	BindCol(&info.uid, sizeof(info.uid));

	wchar_t name[Define::CharacterLimit_MaxNameLen + 1] = L"";
	BindCol((wchar_t**)&name, sizeof(name));

	BindCol(&info.level, sizeof(info.level));
	BindCol((uint8_t*)&info.job, sizeof(info.job));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);

		if (IsSuccess())
		{
			info.name = STRING_MANAGER.Narrow(name);
			infoList.push_back(info);
		}
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
	return LoadAllCharacterGear(accountUID, infoList);
}

bool GameDB::LoadAllCharacterGear(const int64_t accountUID, std::vector<LoginPacket::CharacterInfoT>& infoList)
{
	for (auto& d : infoList)
	{
		if (!LoadCharacterGear(accountUID, d))
			return false;
	}

	return true;
}

bool GameDB::LoadCharacterGear(const int64_t accountUID, LoginPacket::CharacterInfoT& info)
{
	Prepare(L"LoadCharacterGear");
	BindArgument(accountUID);
	BindArgument(info.uid);

	if (!Execute())
	{
		CORE_ERROR_LOG("accountUID: {} uid: {}", accountUID, info.uid);
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
			info.gear.info[count++] = gear;
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
		CORE_ERROR_LOG("accountUID: {}", accountUID);
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return 0;
	}

	uint8_t maxCharacterSlotCount = Define::CharacterLimit_EmptyCharacterSlot;
	BindCol(&maxCharacterSlotCount, sizeof(maxCharacterSlotCount));

	if(IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
		if (IsNoData())
		{
			SQLFreeStmt(this->hstmt, SQL_CLOSE);
			UpdateMaxCharacterSlotCount(accountUID, maxCharacterSlotCount);
			return maxCharacterSlotCount;
		}
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
		CORE_ERROR_LOG("accountUID: {}", accountUID);
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return;
	}

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}

bool GameDB::CreateCharacter(const int64_t accountUID, std::wstring_view name, LoginPacket::CharacterInfoT& info)
{
	info.uid = CORE_UID_GENERATOR.GetUID(dbInfo);

	DATA_MANAGER.GetCharacterCreateGear(this->dbInfo, info.job, info.gear.info);

	Prepare(L"CreateCharacter");
	BindArgument(accountUID);
	BindArgument(info.uid);
	BindArgument(this->dbInfo.serverID);
	BindArgument(name.data());
	BindArgument(info.level);
	BindArgument(info.job);

	int32_t HP = 0, MP = 0;
	DATA_MANAGER.GetCharacterCreateHPMP(info.job, HP, MP);
	BindArgument(HP);
	BindArgument(MP);

	NativeInfo::Stat stat = DATA_MANAGER.GetCharacterCreateStat(info.job);
	for (int32_t i = 0; i < Define::StatType_END; ++i)
		BindArgument(stat.value[i]);

	for (int32_t i = 0; i < Define::GearType_END; ++i)
	{
		BindArgument(info.gear.info[i].itemUID);
		BindArgument(info.gear.info[i].itemID);
	}

	if (!Execute())
	{
		CORE_ERROR_LOG("accountUID: {}", accountUID);
		SQLFreeStmt(this->hstmt, SQL_CLOSE);
		return false;
	}

	bool isSuccess = false;
	int64_t characterUID = 0;

	BindCol(&isSuccess, sizeof(isSuccess));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return isSuccess;
}

bool GameDB::DeleteCharacter(const int64_t accountUID, const int64_t characterUID)
{
	Prepare(L"DeleteCharacter");
	BindArgument(accountUID);
	BindArgument(characterUID);
	if (!Execute())
	{
		CORE_ERROR_LOG("accountUID: {} characterUID: {}", accountUID, characterUID);
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
