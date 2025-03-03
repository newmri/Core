#include "Include.h"

IMPLEMENT_SINGLETON(GameServerManager)

thread_local std::shared_ptr<CoreWorldDB> GameServerManager::worldDB;
thread_local std::shared_ptr<AccountDB> GameServerManager::accountDB;
thread_local std::shared_ptr<GameDB> GameServerManager::gameDB;

void GameServerManager::Init(void)
{
	auto serverInfo = CORE_SERVER_CONFIG.GetServerInfo();

	GetWorldDB()->GetServerInfo(serverInfo);

	this->gameServer = std::make_unique<GameServer>(serverInfo->serverPort);

	CORE_ABILITY_DATA_MANAGER.Load();
	CORE_EFFECT_DATA_MANAGER.Load();
	CORE_ITEM_DATA_MANAGER.Load();

	CHARACTER_DATA_MANAGER.Load();

	ZONE_MANAGER.AddZone(0);

	GetAccountDB();
}

void GameServerManager::Release(void)
{
	GetInstance().~GameServerManager();
}

void GameServerManager::Run(void)
{
	this->gameServer->Run();
}

void GameServerManager::Stop(void)
{
	this->gameServer->Stop();
}

std::shared_ptr<CoreWorldDB> GameServerManager::GetWorldDB(void)
{
	if (IS_SAME(nullptr, this->worldDB))
		MakeWorldDB();

	return this->worldDB;
}

std::shared_ptr<AccountDB> GameServerManager::GetAccountDB(void)
{
	if (IS_SAME(nullptr, this->accountDB))
		MakeAccountDB();

	return this->accountDB;
}

std::shared_ptr<GameDB> GameServerManager::GetGameDB(void)
{
	if (IS_SAME(nullptr, this->gameDB))
		MakeGameDB();

	return this->gameDB;
}

void GameServerManager::MakeWorldDB(void)
{
	const auto serverConfig = CORE_SERVER_CONFIG.GetServerConfig();

	this->worldDB = std::make_shared<CoreWorldDB>(
		DBInfoBuilder()
		.SetDBName(STRING_MANAGER.Widen(ENUM_TO_STR(World)))
		.SetCountryCode(serverConfig->countryCode)
		.SetWorldID(serverConfig->worldID)
		.SetServerID(serverConfig->serverID)
		.SetDBID(0)
		.Build()
	);

	this->worldDB->Init();
}

void GameServerManager::MakeAccountDB(void)
{
	const auto serverConfig = CORE_SERVER_CONFIG.GetServerConfig();

	this->accountDB = std::make_shared<AccountDB>(
		DBInfoBuilder()
		.SetDBName(STRING_MANAGER.Widen(ENUM_TO_STR(Account)))
		.SetCountryCode(serverConfig->countryCode)
		.SetWorldID(serverConfig->worldID)
		.SetServerID(serverConfig->serverID)
		.SetDBID(0)
		.Build()
	);

	this->accountDB->Init();
}

void GameServerManager::MakeGameDB(void)
{
	const auto serverConfig = CORE_SERVER_CONFIG.GetServerConfig();

	this->gameDB = std::make_shared<GameDB>(DBInfoBuilder()
		.SetDBName(STRING_MANAGER.Widen(ENUM_TO_STR(Game)))
		.SetCountryCode(serverConfig->countryCode)
		.SetWorldID(serverConfig->worldID)
		.SetServerID(serverConfig->serverID)
		.SetDBID(0)
		.Build()
	);

	this->gameDB->Init();
}

void GameServerManager::Close(std::shared_ptr<CoreClientSession> session)
{
	this->gameServer->Close(session);
}