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

CoreItemUID GameDB::LoadItemUID(const int16_t serverID)
{
	Prepare(L"LoadItemUID");
	BindArgument(serverID);
	Execute();

	CoreItemUID itemUID;
	BindCol(&itemUID.uid, sizeof(itemUID.uid));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return itemUID;
}

bool GameDB::LoadCharacter(const int64_t accountUID, const int64_t uid, Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo)
{
	Prepare(L"LoadGameCharacter");
	BindArgument(accountUID);
	BindArgument(uid);
	Execute();

	wchar_t name[Define::CharacterLimit_MaxNameLen + 1] = L"";
	BindCol((wchar_t**)&name, sizeof(name));
	BindCol(&creatureInfo.level, sizeof(creatureInfo.level));
	BindCol((uint8_t*)&characterInfo.job, sizeof(characterInfo.job));
	BindCol(&creatureInfo.exp, sizeof(creatureInfo.exp));

	for (int i = 0; i < Define::GearType_END; ++i)
		BindCol(&characterInfo.gear.index[i], sizeof(characterInfo.gear.index[i]));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
		if (IsSuccess())
		{
			SQLFreeStmt(this->hstmt, SQL_CLOSE);

			characterInfo.name = STRING_MANAGER.Narrow(name);
			return LoadCharacterStat(accountUID, uid, creatureInfo, characterInfo);
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
	Execute();

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
	Execute();

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}

void GameDB::Logout(const int64_t uid)
{
	Prepare(L"Logout");
	BindArgument(uid);
	BindArgument(GAME_SERVER.GetWorldID());
	BindArgument(GAME_SERVER.GetServerID());
	Execute();

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);
}
