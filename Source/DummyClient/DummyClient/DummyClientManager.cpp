#include "Include.h"

IMPLEMENT_SINGLETON(DummyClientManager)

void DummyClientManager::Init(void)
{
	for (int64_t i = 0; i < 100; ++i)
	{
		auto client = std::make_shared<LoginClient>(i);
		client->Connect("127.0.0.1", "7000");
		boost::asio::io_context& ioContext = client->GetContext();
		boost::thread t(boost::bind(&boost::asio::io_context::run, &ioContext));

		WRITE_LOCK(this->mutex);
		this->clientList[i] = client;
	}
}

void DummyClientManager::Release(void)
{
	GetInstance().~DummyClientManager();
}

void DummyClientManager::Run(void)
{

}

void DummyClientManager::Stop(void)
{
	
}

void DummyClientManager::DeleteLoginClient(const int64_t uid)
{
	WRITE_LOCK(this->mutex);

	this->clientList.erase(uid);

	if (this->clientList.empty())
	{
		std::cout << "\n" << std::endl;
		CORE_ALL_LOG(LogType::LOG_ERROR, "Cannot Connect, Shutdown");
		Sleep(3000);
		abort();
	}
}
