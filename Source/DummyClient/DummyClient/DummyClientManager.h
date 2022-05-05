#pragma once

#define DUMMY_CLIENT GET_INSTANCE(DummyClientManager)

class DummyClientManager
{
	DECLARE_SINGLETON(DummyClientManager)

public:
	void Run(void);
	void Stop(void);

public:
	void AddLoginClient(std::shared_ptr<LoginClient> client);
	void DeleteLoginClient(std::shared_ptr<LoginClient> client);

private:
	CoreList<std::shared_ptr<LoginClient>> clientList;
};