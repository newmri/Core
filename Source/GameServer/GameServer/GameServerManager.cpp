#include "Include.h"

IMPLEMENT_SINGLETON(GameServerManager)

thread_local std::shared_ptr<WorldDB> GameServerManager::worldDB;
thread_local std::shared_ptr<AccountDB> GameServerManager::accountDB;
thread_local std::shared_ptr<GameDB> GameServerManager::gameDB;

void GameServerManager::Init(void)
{
	CSV_LOAD_AND_TO_HAS_MAP("DBConfig.csv", DBConfig, this->dbConfig, Name);
	CSV_LOAD_ONE_ROW("ServerConfig.csv", ServerConfig, this->serverConfig);

	MakeWorldDB();

	this->worldDB->GerServerInfo(this->serverConfig->WorldID, this->serverConfig->ServerID, this->serverInfo);
	this->gameServer = std::make_unique<GameServer>(this->serverInfo.ServerPort);

	CORE_LOG.Log(LogType::LOG_DEBUG, "[Server Type]: " + this->serverConfig->ServerType);
	CORE_LOG.Log(LogType::LOG_DEBUG, "[GroupID]: " + TO_STR(this->serverInfo.GroupID));
	CORE_LOG.Log(LogType::LOG_DEBUG, "[WorldID]: " + TO_STR(this->serverConfig->WorldID));
	CORE_LOG.Log(LogType::LOG_DEBUG, "[ServerID]: " + TO_STR(this->serverConfig->ServerID));

	DATA_MANAGER.Load();
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
	std::string dbName = ENUM_TO_STR(World);
	this->worldDB = std::make_shared<WorldDB>(STRING_MANAGER.Widen(ENUM_TO_STR(World)));
}

void GameServerManager::MakeAccountDB(void)
{
	std::string dbName = ENUM_TO_STR(Account);
	this->accountDB = std::make_shared<AccountDB>(STRING_MANAGER.Widen(ENUM_TO_STR(Account)));
}

void GameServerManager::MakeGameDB(void)
{
	std::string dbName = ENUM_TO_STR(Game);
	this->gameDB = std::make_shared<GameDB>(STRING_MANAGER.Widen(ENUM_TO_STR(Game)));
}


void GameServerManager::Close(std::shared_ptr<CoreClientSession> session)
{
	this->gameServer->Close(session);
}