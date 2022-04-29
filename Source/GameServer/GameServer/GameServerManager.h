#pragma once

#define GAME_SERVER GET_INSTANCE(GameServerManager)

class GameServerManager
{
	DECLARE_SINGLETON(GameServerManager)

public:
	void Run(void);
	void Stop(void);

public:
	std::shared_ptr<WorldDB> GetWorldDB(void);
	std::shared_ptr<AccountDB> GetAccountDB(void);
	std::shared_ptr<GameDB> GetGameDB(void);

public:
	int32_t GetWorldID(void);
	int32_t GetServerID(void);

private:
	void MakeWorldDB(void);
	void MakeAccountDB(void);
	void MakeGameDB(void);

public:
	void Close(std::shared_ptr<CoreClientSession> session);

private:
	std::unique_ptr<GameServer> gameServer;

private:
	std::unordered_map<std::string, std::shared_ptr<DBConfig>> dbConfig;
	std::shared_ptr<ServerConfig> serverConfig;
	ServerInfo serverInfo;

private:
	static thread_local std::shared_ptr<WorldDB> worldDB;
	static thread_local std::shared_ptr<AccountDB> accountDB;
	static thread_local std::shared_ptr<GameDB> gameDB;
};