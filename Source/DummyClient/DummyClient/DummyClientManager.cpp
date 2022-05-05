#include "Include.h"

IMPLEMENT_SINGLETON(DummyClientManager)

void DummyClientManager::Init(void)
{
	for (int32_t i = 0; i < 1; ++i)
	{
		auto client = std::make_shared<LoginClient>();
		client->Connect("127.0.0.1", "7000");
		boost::asio::io_context& ioContext = client->GetContext();
		boost::thread t(boost::bind(&boost::asio::io_context::run, &ioContext));
		AddLoginClient(client);
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

void DummyClientManager::AddLoginClient(std::shared_ptr<LoginClient> client)
{
	this->clientList.push_back(client);
}

void DummyClientManager::DeleteLoginClient(std::shared_ptr<LoginClient> client)
{
	this->clientList.remove(client);

	if (this->clientList.empty())
	{
		std::cout << "\n" << std::endl;
		CORE_ALL_LOG(LogType::LOG_ERROR, "Cannot Connect, Shutdown");
		Sleep(3000);
		abort();
	}
}
