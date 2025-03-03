#pragma once

#define GAME_SERVER GET_INSTANCE(GameServerManager)

class GameServerManager
{
	DECLARE_SINGLETON(GameServerManager)

public:
	void Run(void);
	void Stop(void);

public:
	std::shared_ptr<CoreWorldDB> GetWorldDB(void);
	std::shared_ptr<AccountDB> GetAccountDB(void);
	std::shared_ptr<GameDB> GetGameDB(void);

private:
	void MakeWorldDB(void);
	void MakeAccountDB(void);
	void MakeGameDB(void);

public:
	void Close(std::shared_ptr<CoreClientSession> session);

private:
	std::unique_ptr<GameServer> gameServer;

private:
	static thread_local std::shared_ptr<CoreWorldDB> worldDB;
	static thread_local std::shared_ptr<AccountDB> accountDB;
	static thread_local std::shared_ptr<GameDB> gameDB;
};