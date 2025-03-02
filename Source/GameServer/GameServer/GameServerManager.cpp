#include "Include.h"

IMPLEMENT_SINGLETON(GameServerManager)

thread_local std::shared_ptr<CoreWorldDB> GameServerManager::worldDB;
thread_local std::shared_ptr<AccountDB> GameServerManager::accountDB;
thread_local std::shared_ptr<GameDB> GameServerManager::gameDB;

void GameServerManager::Init(void)
{
	CSV_LOAD_ONE_ROW("ServerConfig.csv", ServerConfig, this->serverConfig);

	GetWorldDB()->GetServerInfo(this->serverInfo);
	this->gameServer = std::make_unique<GameServer>(this->serverInfo.serverPort);

	CORE_LOG(LogType::LOG_DEBUG, "[Server Type]: {}", this->serverConfig->serverType);
	CORE_LOG(LogType::LOG_DEBUG, "[GroupID]: {}", this->serverInfo.groupID);
	CORE_LOG(LogType::LOG_DEBUG, "[CountryID]: {}", ENUM_TO_STR(this->serverConfig->countryID));
	CORE_LOG(LogType::LOG_DEBUG, "[WorldID]: {}", this->serverConfig->worldID);
	CORE_LOG(LogType::LOG_DEBUG, "[ServerID]: {}", this->serverConfig->serverID);

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

uint8_t GameServerManager::GetWorldID(void)
{
	return this->serverConfig->worldID;
}

uint8_t GameServerManager::GetServerID(void)
{
	return this->serverConfig->serverID;
}

void GameServerManager::MakeWorldDB(void)
{
	this->worldDB = std::make_shared<CoreWorldDB>(
		DBInfoBuilder()
		.SetDBName(STRING_MANAGER.Widen(ENUM_TO_STR(World)))
		.SetCountryCode(this->serverConfig->countryCode)
		.SetWorldID(this->serverConfig->worldID)
		.SetServerID(this->serverConfig->serverID)
		.SetDBID(0)
		.Build()
	);

	this->worldDB->Init();
}

void GameServerManager::MakeAccountDB(void)
{
	this->accountDB = std::make_shared<AccountDB>(
		DBInfoBuilder()
		.SetDBName(STRING_MANAGER.Widen(ENUM_TO_STR(Account)))
		.SetCountryCode(this->serverConfig->countryCode)
		.SetWorldID(this->serverConfig->worldID)
		.SetServerID(this->serverConfig->serverID)
		.SetDBID(0)
		.Build()
	);

	this->accountDB->Init();
}

void GameServerManager::MakeGameDB(void)
{
	this->gameDB = std::make_shared<GameDB>(DBInfoBuilder()
		.SetDBName(STRING_MANAGER.Widen(ENUM_TO_STR(Game)))
		.SetCountryCode(this->serverConfig->countryCode)
		.SetWorldID(this->serverConfig->worldID)
		.SetServerID(this->serverConfig->serverID)
		.SetDBID(0)
		.Build()
	);

	this->gameDB->Init();
}

void GameServerManager::Close(std::shared_ptr<CoreClientSession> session)
{
	this->gameServer->Close(session);
}