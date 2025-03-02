#pragma once

#define LOGIN_SERVER GET_INSTANCE(LoginServerManager)

class LoginServerManager
{
	DECLARE_SINGLETON(LoginServerManager)

public:
	void Run(void);
	void Stop(void);

public:
	std::shared_ptr<CoreWorldDB> GetWorldDB(void);
	std::shared_ptr<AccountDB> GetAccountDB(void);
	std::shared_ptr<GameDB> GetGameDB(void);

public:
	uint8_t GetWorldID(void);
	uint8_t GetServerID(void);

private:
	void MakeWorldDB(void);
	void MakeAccountDB(void);
	void MakeGameDB(void);

public:
	void Close(std::shared_ptr<CoreClientSession> session);

private:
	std::unique_ptr<LoginServer> loginServer;

private:
	std::shared_ptr<ServerConfig> serverConfig;
	ServerInfo serverInfo;

private:
	static thread_local std::shared_ptr<CoreWorldDB> worldDB;
	static thread_local std::shared_ptr<AccountDB> accountDB;
	static thread_local std::shared_ptr<GameDB> gameDB;
};