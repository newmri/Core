#include "Include.h"
#include "boost/current_function.hpp"

GameDB::GameDB(std::wstring_view dbName) : CoreDB(dbName)
{

}

GameDB::~GameDB()
{
	Release();
}

void GameDB::Release(void)
{
}

void GameDB::LoadCharacter(const int64_t accountUID, std::vector<LoginPacket::CharacterInfoT>& infoList)
{
	Prepare(L"LoadLoginCharacter");
	BindArgument(accountUID);
	Execute();

	LoginPacket::CharacterInfoT info;
	BindCol(&info.uid, sizeof(info.uid));

	wchar_t name[Define::CharacterLimit_MaxNameLen + 1] = L"";
	BindCol((wchar_t**)&name, sizeof(name));

	BindCol(&info.level, sizeof(info.level));
	BindCol((uint8_t*)&info.job, sizeof(info.job));

	for (int32_t i = 0; i < Define::GearType_END; ++i)
		BindCol(&info.gear.index[i], sizeof(info.gear.index[i]));

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
}

uint8_t GameDB::LoadMaxCharacterSlotCount(const int64_t accountUID)
{
	Prepare(L"LoadMaxCharacterSlotCount");
	BindArgument(accountUID);
	Execute();

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
	Execute();

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}

bool GameDB::CreateCharacter(const int64_t accountUID, std::wstring_view name, LoginPacket::CharacterInfoT& info)
{
	Prepare(L"CreateCharacter");
	BindArgument(accountUID);
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

	DATA_MANAGER.GetCharacterCreateGear(info.job, info.gear.index);
	for (int32_t i = 0; i < Define::GearType_END; ++i)
		BindArgument(info.gear.index[i]);

	Execute();

	bool isSuccess = false;
	int64_t characterUID = 0;

	BindCol(&isSuccess, sizeof(isSuccess));
	BindCol(&characterUID, sizeof(characterUID));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);

		if (IsSuccess())
			info.uid = characterUID;
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return isSuccess;
}

bool GameDB::DeleteCharacter(const int64_t accountUID, const int64_t characterUID)
{
	Prepare(L"DeleteCharacter");
	BindArgument(accountUID);
	BindArgument(characterUID);
	Execute();

	bool isSuccess = false;
	BindCol(&isSuccess, sizeof(isSuccess));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return isSuccess;
}
