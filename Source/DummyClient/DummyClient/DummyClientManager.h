#pragma once

#define DUMMY_CLIENT GET_INSTANCE(DummyClientManager)

enum ConnectState
{
	LOGIN,
	GAME
};

#pragma pack(push, 1)
struct DummyClientConfig
{
	uint8_t WorldID = 0;
	int32_t MaxConnectionCount = 0;
};
#pragma pack(pop)

class DummyClientManager
{
	DECLARE_SINGLETON(DummyClientManager)

public:
	void Run(void);
	void Stop(void);

public:
	uint8_t GetWorldID(void);
	int32_t GetMaxConnectionCount(void);

#pragma region LoginServer
public:
	void ConnectToLoginServer(void);

public:
	void OnLoginServerConnected(std::shared_ptr<CoreServerSession> session);
	void OnLoginServerDisconnected(std::shared_ptr<CoreServerSession> session);
	void LoginServerProcessPacket(std::shared_ptr<CoreServerSession> session, const uint8_t* data, size_t size);

public:
	void ShowConnectedLoginClientCount(void);

private:
	int32_t GetConnectedLoginClientCount(void);

public:
	void DeleteLoginClient(const int64_t oid);
	void DeleteAllLoginClient(void);
#pragma endregion

#pragma region GameServer
public:
	void ConnectToGameServer(std::shared_ptr<CoreServerSession> session, const int64_t characterUID, std::string_view characterName);

public:
	void OnGameServerConnected(std::shared_ptr<CoreServerSession> session);
	void OnGameServerDisconnected(std::shared_ptr<CoreServerSession> session);
	void GameServerProcessPacket(std::shared_ptr<CoreServerSession> session, const uint8_t* data, size_t size);

public:
	void ShowConnectedGameClientCount(void);

private:
	int32_t GetConnectedGameClientCount(void);

public:
	void DeleteGameClient(const int64_t oid);
#pragma endregion

public:
	bool IsMyPlayer(std::string_view characterName);

private:
	void Shutdown(void);

private:
	std::shared_ptr<DummyClientConfig> dummyClientConfig;

private:
	std::unique_ptr<LoginPacketHandler> loginHandler;
	std::vector<std::thread> loginThread;
	std::shared_mutex loginMutex;
	std::unordered_map<int64_t, std::shared_ptr<LoginClient>> loginClientList;

private:
	std::unique_ptr<GamePacketHandler> gameHandler;
	std::shared_mutex gameMutex;
	std::unordered_map<int64_t, std::shared_ptr<GameClient>> gameClientList;

private:
	ConnectState connectState = ConnectState::LOGIN;
};