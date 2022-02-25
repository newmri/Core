#include "Include.h"

IMPLEMENT_SINGLETON(LoginServerManager)

thread_local std::shared_ptr<WorldDB> LoginServerManager::worldDB;
thread_local std::shared_ptr<AccountDB> LoginServerManager::accountDB;

void LoginServerManager::Init(void)
{
	CSV_LOAD_AND_TO_HAS_MAP("DBConfig.csv", DBConfig, this->dbConfig, Name);
	CSV_LOAD_ONE_ROW("ServerConfig.csv", ServerConfig, this->serverConfig);

	MakeWorldDB();

	this->worldDB->GerServerInfo(this->serverConfig->WorldID, this->serverConfig->ServerID, this->serverInfo);
	this->loginServer = std::make_unique<LoginServer>(this->serverInfo.ServerPort);

	CORE_LOG.Log(LogType::LOG_DEBUG, "[Server Type]: " + this->serverConfig->ServerType);
	CORE_LOG.Log(LogType::LOG_DEBUG, "[GroupID]: " + TO_STR(this->serverInfo.GroupID));
	CORE_LOG.Log(LogType::LOG_DEBUG, "[WorldID]: " + TO_STR(this->serverConfig->WorldID));
	CORE_LOG.Log(LogType::LOG_DEBUG, "[ServerID]: " + TO_STR(this->serverConfig->ServerID));
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

std::shared_ptr<AccountDB> LoginServerManager::GetAccountDB(void)
{
	if (IS_SAME(nullptr, this->accountDB))
		MakeAccountDB();

	return this->accountDB;
}

void LoginServerManager::MakeWorldDB(void)
{
	std::string dbName = ENUM_TO_STR(World);
	this->worldDB = std::make_shared<WorldDB>(STRING_MANAGER.Widen(ENUM_TO_STR(World)));
}

void LoginServerManager::MakeAccountDB(void)
{
	std::string dbName = ENUM_TO_STR(Account);
	this->accountDB = std::make_shared<AccountDB>(STRING_MANAGER.Widen(ENUM_TO_STR(Account)));
}

void LoginServerManager::Close(std::shared_ptr<CoreClientSession> session)
{
	this->loginServer->Close(session);
}