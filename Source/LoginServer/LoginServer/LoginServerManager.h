#pragma once

#define LOGIN_SERVER GET_INSTANCE(LoginServerManager)

class LoginServerManager
{
	DECLARE_SINGLETON(LoginServerManager)

public:
	void Run(void);
	void Stop(void);

public:
	std::shared_ptr<AccountDB> GetAccountDB(void);

private:
	void MakeWorldDB(void);
	void MakeAccountDB(void);

private:
	std::unique_ptr<LoginServer> loginServer;

private:
	std::unordered_map<std::string, std::shared_ptr<DBConfig>> dbConfig;
	std::shared_ptr<ServerConfig> serverConfig;
	ServerInfo serverInfo;

private:
	static thread_local std::shared_ptr<WorldDB> worldDB;
	static thread_local std::shared_ptr<AccountDB> accountDB;
};