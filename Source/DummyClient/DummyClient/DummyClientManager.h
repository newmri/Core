#pragma once

#define DUMMY_CLIENT GET_INSTANCE(DummyClientManager)

struct DummyClientConfig
{
	int32_t WorldID = 0;
	int32_t MaxConnectionCount = 0;
};

class DummyClientManager
{
	DECLARE_SINGLETON(DummyClientManager)

public:
	void Run(void);
	void Stop(void);

public:
	int32_t GetWorldID(void);
	int32_t GetMaxConnectionCount(void);

#pragma region LoginServer
public:
	void ConnectToLoginServer(void);
	void ShowConnectedLoginClientCount(void);

private:
	int32_t GetConnectedLoginClientCount(void);

public:
	void DeleteLoginClient(const int64_t oid);
#pragma endregion

#pragma region GameServer
public:
	void ConnectToGameServer(const int64_t oid, const int64_t accountUID, const int32_t token, const int64_t characterUID);
	void ShowConnectedGameClientCount(void);

private:
	int32_t GetConnectedGameClientCount(void);

public:
	void DeleteGameClient(const int64_t oid);
#pragma endregion

private:
	std::shared_ptr<DummyClientConfig> dummyClientConfig;

private:
	std::shared_mutex loginMutex;
	std::map<int64_t, std::shared_ptr<LoginClient>> loginClientList;

private:
	std::shared_mutex gameMutex;
	std::map<int64_t, std::shared_ptr<GameClient>> gameClientList;

};