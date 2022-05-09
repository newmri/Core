#pragma once

#define DUMMY_CLIENT GET_INSTANCE(DummyClientManager)

class DummyClientManager
{
	DECLARE_SINGLETON(DummyClientManager)

public:
	void Run(void);
	void Stop(void);

public:
	void DeleteLoginClient(const int64_t uid);

private:
	std::shared_mutex mutex;
	std::map<int64_t, std::shared_ptr<LoginClient>> clientList;
};