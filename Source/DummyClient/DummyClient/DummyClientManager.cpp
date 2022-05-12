#include "Include.h"

IMPLEMENT_SINGLETON(DummyClientManager)

void DummyClientManager::Init(void)
{
	CSV_LOAD_ONE_ROW("DummyClientConfig.csv", DummyClientConfig, this->dummyClientConfig);
	CORE_ALL_LOG(LogType::LOG_DEBUG, "[World ID]: " + TO_STR(GetWorldID()));
	CORE_ALL_LOG(LogType::LOG_DEBUG, "[MaxConnection]: " + TO_STR(GetMaxConnectionCount()));

}

void DummyClientManager::Release(void)
{
	GetInstance().~DummyClientManager();
}

void DummyClientManager::Run(void)
{
	ConnectToLoginServer();
	ShowConnectedLoginClientCount();
}

void DummyClientManager::Stop(void)
{
	
}

Define::ServerType DummyClientManager::GetCurrServerType(void)
{
	return this->currServerType;
}

void DummyClientManager::SetCurrServerType(const Define::ServerType currServerType)
{
	WRITE_LOCK(this->mutex);
	this->currServerType = currServerType;
}

void DummyClientManager::ConnectToLoginServer(void)
{
	int32_t maxConnectionCount = GetMaxConnectionCount();
	for (int32_t i = 0; i < maxConnectionCount; ++i)
	{
		auto client = std::make_shared<LoginClient>(i);
		client->Connect();

		WRITE_LOCK(this->mutex);
		this->loginClientList[i] = client;
	}

	Sleep(SEC);
}

void DummyClientManager::ShowConnectedLoginClientCount(void)
{
	CORE_ALL_LOG(LogType::LOG_DEBUG, "[Connected To LoginServer]: " + TO_STR(GetConnectedLoginClientCount()));
}

int32_t DummyClientManager::GetConnectedLoginClientCount(void)
{
	int32_t connectedCount = 0;

	READ_LOCK(this->mutex);

	auto iter_begin = this->loginClientList.begin();
	auto iter_end = this->loginClientList.end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		if ((*iter_begin).second->IsConnected())
			++connectedCount;
	}

	return connectedCount;
}

int32_t DummyClientManager::GetWorldID(void)
{
	return this->dummyClientConfig.get()->WorldID;
}

int32_t DummyClientManager::GetMaxConnectionCount(void)
{
	return this->dummyClientConfig.get()->MaxConnectionCount;
}

void DummyClientManager::DeleteLoginClient(const int64_t uid)
{
	WRITE_LOCK(this->mutex);

	this->loginClientList.erase(uid);

	if (this->loginClientList.empty() && IS_SAME(Define::ServerType_Login, GetCurrServerType()))
	{
		std::cout << "\n" << std::endl;
		CORE_ALL_LOG(LogType::LOG_ERROR, "Cannot Connect, Shutdown");
		Sleep(3000);
		abort();
	}
}
