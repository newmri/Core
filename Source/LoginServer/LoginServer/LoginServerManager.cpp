#include "Include.h"

IMPLEMENT_SINGLETON(LoginServerManager)

thread_local std::shared_ptr<CoreWorldDB> LoginServerManager::worldDB;
thread_local std::shared_ptr<AccountDB> LoginServerManager::accountDB;
thread_local std::shared_ptr<GameDB> LoginServerManager::gameDB;

void LoginServerManager::Init(void)
{
	CSV_LOAD_AND_TO_HASH_MAP("DBConfig.csv", DBConfig, this->dbConfig, Name);
	CSV_LOAD_ONE_ROW("ServerConfig.csv", ServerConfig, this->serverConfig);

	GetWorldDB()->GetServerInfo(this->serverInfo);
	this->loginServer = std::make_unique<LoginServer>(this->serverInfo.ServerPort);

	CORE_LOG.Log(LogType::LOG_DEBUG, "[Server Type]: " + this->serverConfig->ServerType);
	CORE_LOG.Log(LogType::LOG_DEBUG, "[GroupID]: " + TO_STR(this->serverInfo.GroupID));
	CORE_LOG.Log(LogType::LOG_DEBUG, "[WorldID]: " + TO_STR(this->serverConfig->WorldID));
	CORE_LOG.Log(LogType::LOG_DEBUG, "[ServerID]: " + TO_STR(this->serverConfig->ServerID));

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
	return this->serverConfig->WorldID;
}

uint8_t LoginServerManager::GetServerID(void)
{
	return this->serverConfig->ServerID;
}

void LoginServerManager::MakeWorldDB(void)
{
	this->worldDB = std::make_shared<CoreWorldDB>(STRING_MANAGER.Widen(ENUM_TO_STR(World)), this->serverConfig->WorldID, this->serverConfig->ServerID);
	this->worldDB->Init();
}

void LoginServerManager::MakeAccountDB(void)
{
	this->accountDB = std::make_shared<AccountDB>(STRING_MANAGER.Widen(ENUM_TO_STR(Account)), this->serverConfig->WorldID, this->serverConfig->ServerID);
	this->accountDB->Init();
}

void LoginServerManager::MakeGameDB(void)
{
	this->gameDB = std::make_shared<GameDB>(STRING_MANAGER.Widen(ENUM_TO_STR(Game)), this->serverConfig->WorldID, this->serverConfig->ServerID);
	this->gameDB->Init();
}

void LoginServerManager::Close(std::shared_ptr<CoreClientSession> session)
{
	this->loginServer->Close(session);
}