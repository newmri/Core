#include "Include.h"

IMPLEMENT_SINGLETON(LoginServerManager)

thread_local std::shared_ptr<CoreWorldDB> LoginServerManager::worldDB;
thread_local std::shared_ptr<AccountDB> LoginServerManager::accountDB;
thread_local std::shared_ptr<GameDB> LoginServerManager::gameDB;

void LoginServerManager::Init(void)
{
	CSV_LOAD_ONE_ROW("ServerConfig.csv", ServerConfig, this->serverConfig);

	GetWorldDB()->GetServerInfo(this->serverInfo);
	this->loginServer = std::make_unique<LoginServer>(this->serverInfo.serverPort);

	CORE_LOG(LogType::LOG_DEBUG, "[Server Type]: {}", this->serverConfig->serverType);
	CORE_LOG(LogType::LOG_DEBUG, "[GroupID]: {}", this->serverInfo.groupID);
	CORE_LOG(LogType::LOG_DEBUG, "[CountryID]: {}", ENUM_TO_STR(this->serverConfig->countryCode));
	CORE_LOG(LogType::LOG_DEBUG, "[WorldID]: {}", this->serverConfig->worldID);
	CORE_LOG(LogType::LOG_DEBUG, "[ServerID]: {}", this->serverConfig->serverID);

	DATA_MANAGER.Load();
	CORE_ITEM_DATA_MANAGER.Load();

	GetAccountDB();
}

void LoginServerManager::Release(void)
{
	GetInstance().~LoginServerManager();
}

void LoginServerManager::Run(void)
{
	this->loginServer->Run();
}

void LoginServerManager::Stop(void)
{
	this->loginServer->Stop();
}

std::shared_ptr<CoreWorldDB> LoginServerManager::GetWorldDB(void)
{
	if (IS_SAME(nullptr, this->worldDB))
		MakeWorldDB();

	return this->worldDB;
}

std::shared_ptr<AccountDB> LoginServerManager::GetAccountDB(void)
{
	if (IS_SAME(nullptr, this->accountDB))
		MakeAccountDB();

	return this->accountDB;
}

std::shared_ptr<GameDB> LoginServerManager::GetGameDB(void)
{
	if (IS_SAME(nullptr, this->gameDB))
		MakeGameDB();

	return this->gameDB;
}

uint8_t LoginServerManager::GetWorldID(void)
{
	return this->serverConfig->worldID;
}

uint8_t LoginServerManager::GetServerID(void)
{
	return this->serverConfig->serverID;
}

void LoginServerManager::MakeWorldDB(void)
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

void LoginServerManager::MakeAccountDB(void)
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

void LoginServerManager::MakeGameDB(void)
{
	this->gameDB = std::make_shared<GameDB>(
		DBInfoBuilder()
		.SetDBName(STRING_MANAGER.Widen(ENUM_TO_STR(Game)))
		.SetCountryCode(this->serverConfig->countryCode)
		.SetWorldID(this->serverConfig->worldID)
		.SetServerID(this->serverConfig->serverID)
		.SetDBID(0)
		.Build()
	);

	this->gameDB->Init();
}

void LoginServerManager::Close(std::shared_ptr<CoreClientSession> session)
{
	this->loginServer->Close(session);
}