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
	void ConnectToLoginServer(void);
	void ShowConnectedLoginClientCount(void);

private:
	int32_t GetConnectedLoginClientCount(void);

public:
	int32_t GetWorldID(void);
	int32_t GetMaxConnectionCount(void);

public:
	void DeleteLoginClient(const int64_t uid);

private:
	std::shared_ptr<DummyClientConfig> dummyClientConfig;

private:
	std::shared_mutex mutex;
	std::map<int64_t, std::shared_ptr<LoginClient>> loginClientList;
};