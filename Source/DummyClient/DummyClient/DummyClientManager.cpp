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
	Sleep(SEC);
	ShowConnectedGameClientCount();
}

void DummyClientManager::Stop(void)
{
	
}

int32_t DummyClientManager::GetWorldID(void)
{
	return this->dummyClientConfig.get()->WorldID;
}

int32_t DummyClientManager::GetMaxConnectionCount(void)
{
	return this->dummyClientConfig.get()->MaxConnectionCount;
}

void DummyClientManager::ConnectToLoginServer(void)
{
	int32_t maxConnectionCount = GetMaxConnectionCount();
	for (int32_t i = 0; i < maxConnectionCount; ++i)
	{
		auto client = std::make_shared<LoginClient>(i);
		client->Connect();

		WRITE_LOCK(this->loginMutex);
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

	READ_LOCK(this->loginMutex);

	auto iter_begin = this->loginClientList.begin();
	auto iter_end = this->loginClientList.end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		if ((*iter_begin).second->IsConnected())
			++connectedCount;
	}

	return connectedCount;
}

void DummyClientManager::DeleteLoginClient(const int64_t oid)
{
	WRITE_LOCK(this->loginMutex);

	this->loginClientList.erase(oid);

	if (this->loginClientList.empty())
	{
		std::cout << "\n" << std::endl;
		CORE_ALL_LOG(LogType::LOG_ERROR, "Cannot Connect, Shutdown");
		Sleep(1000);
		abort();
	}
}

void DummyClientManager::ConnectToGameServer(const int64_t oid, const int64_t accountUID, const int32_t token, const int64_t characterUID)
{
	auto client = std::make_shared<GameClient>(oid, accountUID, token, characterUID);
	client->Connect();

	WRITE_LOCK(this->gameMutex);
	this->gameClientList[oid] = client;
}

void DummyClientManager::ShowConnectedGameClientCount(void)
{
	CORE_ALL_LOG(LogType::LOG_DEBUG, "[Connected To GameServer]: " + TO_STR(GetConnectedGameClientCount()));
}

int32_t DummyClientManager::GetConnectedGameClientCount(void)
{
	int32_t connectedCount = 0;

	READ_LOCK(this->gameMutex);

	auto iter_begin = this->gameClientList.begin();
	auto iter_end = this->gameClientList.end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		if ((*iter_begin).second->IsConnected())
			++connectedCount;
	}

	return connectedCount;
}

void DummyClientManager::DeleteGameClient(const int64_t oid)
{
	WRITE_LOCK(this->gameMutex);

	this->gameClientList.erase(oid);

	if (this->gameClientList.empty())
	{
		std::cout << "\n" << std::endl;
		CORE_ALL_LOG(LogType::LOG_ERROR, "GameClient is Zero, Shutdown");
		abort();
	}
}
