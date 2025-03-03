#include "Include.h"

IMPLEMENT_SINGLETON(LoginServerManager)

thread_local std::shared_ptr<CoreWorldDB> LoginServerManager::worldDB;
thread_local std::shared_ptr<AccountDB> LoginServerManager::accountDB;
thread_local std::shared_ptr<GameDB> LoginServerManager::gameDB;

void LoginServerManager::Init(void)
{
	auto serverInfo = CORE_SERVER_CONFIG.GetServerInfo();

	GetWorldDB()->GetServerInfo(serverInfo);

	this->loginServer = std::make_unique<LoginServer>(serverInfo->serverPort);

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

void LoginServerManager::MakeWorldDB(void)
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

void LoginServerManager::MakeAccountDB(void)
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

void LoginServerManager::MakeGameDB(void)
{
	const auto serverConfig = CORE_SERVER_CONFIG.GetServerConfig();

	this->gameDB = std::make_shared<GameDB>(
		DBInfoBuilder()
		.SetDBName(STRING_MANAGER.Widen(ENUM_TO_STR(Game)))
		.SetCountryCode(serverConfig->countryCode)
		.SetWorldID(serverConfig->worldID)
		.SetServerID(serverConfig->serverID)
		.SetDBID(0)
		.Build()
	);

	this->gameDB->Init();
}

void LoginServerManager::Close(std::shared_ptr<CoreClientSession> session)
{
	this->loginServer->Close(session);
}