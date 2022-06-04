#include "Include.h"

IMPLEMENT_SINGLETON(DummyClientManager)

void DummyClientManager::Init(void)
{
	CSV_LOAD_ONE_ROW("DummyClientConfig.csv", DummyClientConfig, this->dummyClientConfig);
	CORE_ALL_LOG(LogType::LOG_DEBUG, "[World ID]: " + TO_STR(GetWorldID()));
	CORE_ALL_LOG(LogType::LOG_DEBUG, "[MaxConnection]: " + TO_STR(GetMaxConnectionCount()));

	this->loginHandler = std::make_unique<LoginPacketHandler>();
	this->gameHandler = std::make_unique<GamePacketHandler>();

	CHARACTER_DATA_MANAGER.Load();
	ZONE_MANAGER.AddZone(0);
}

void DummyClientManager::Release(void)
{
	GetInstance().~DummyClientManager();
}

void DummyClientManager::Run(void)
{
	CORE_ALL_LOG(LogType::LOG_DEBUG, "Trying to connect to LoginServer..... ");
	ConnectToLoginServer();
	ShowConnectedLoginClientCount();
	CORE_ALL_LOG(LogType::LOG_DEBUG, "Trying to connect to GameServer..... ");
	ShowConnectedGameClientCount();
	DeleteAllLoginClient();
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
		if (!client->Connect())
			Shutdown();

		WRITE_LOCK(this->loginMutex);
		this->loginClientList[i] = client;
	}
}

void DummyClientManager::OnLoginServerConnected(std::shared_ptr<CoreServerSession> session)
{
	LOGIN_PACKET_SEND_MANAGER.Clear();
	auto message = LoginPacket::CreateCS_LOGIN_REQ(LOGIN_PACKET_SEND_MANAGER.builder, session->GetAccountUID(), session->GetToken());
	LOGIN_PACKET_SEND_MANAGER.Send(session, LoginPacket::Packet_CS_LOGIN_REQ, message.Union());
}

void DummyClientManager::OnLoginServerDisconnected(std::shared_ptr<CoreServerSession> session)
{
	DeleteLoginClient(session->GetOID());
}

void DummyClientManager::LoginServerProcessPacket(std::shared_ptr<CoreServerSession> session, const uint8_t* data, size_t size)
{
	auto verifier = flatbuffers::Verifier(data, size);

	if (LoginPacket::VerifyRootBuffer(verifier))
	{
		auto root = LoginPacket::GetRoot(data);

		this->loginHandler->Handle(session, root->packet_type(), root->packet());
	}
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
	this->loginClientList[oid]->Stop();
	this->loginClientList.erase(oid);
}

void DummyClientManager::DeleteAllLoginClient(void)
{
	CORE_ALL_LOG(LogType::LOG_DEBUG, "All LoginClient is Deleted");

	WRITE_LOCK(this->loginMutex);

	auto iter_begin = this->loginClientList.begin();
	auto iter_end = this->loginClientList.end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		iter_begin->second->Stop();
	}

	this->loginClientList.clear();
}

void DummyClientManager::ConnectToGameServer(std::shared_ptr<CoreServerSession> session, const int64_t characterUID, std::string_view characterName)
{
	WRITE_LOCK(this->gameMutex);

	auto client = std::make_shared<GameClient>(session, characterUID, characterName);
	if (!client->Connect())
		Shutdown();

	auto oid = session->GetOID();

	this->gameClientList[oid] = client;
}

void DummyClientManager::OnGameServerConnected(std::shared_ptr<CoreServerSession> session)
{
	GAME_PACKET_SEND_MANAGER.Clear();
	auto message = GamePacket::CreateCS_LOGIN_REQ(GAME_PACKET_SEND_MANAGER.builder, session->GetAccountUID(), session->GetCharacterUID(), session->GetToken());
	GAME_PACKET_SEND_MANAGER.Send(session, GamePacket::Packet_CS_LOGIN_REQ, message.Union());
}

void DummyClientManager::OnGameServerDisconnected(std::shared_ptr<CoreServerSession> session)
{
	DUMMY_CLIENT.DeleteGameClient(session->GetOID());
}

void DummyClientManager::GameServerProcessPacket(std::shared_ptr<CoreServerSession> session, const uint8_t* data, size_t size)
{
	auto verifier = flatbuffers::Verifier(data, size);

	if (GamePacket::VerifyRootBuffer(verifier))
	{
		auto root = GamePacket::GetRoot(data);

		this->gameHandler->Handle(session, root->packet_type(), root->packet());
	}
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

	OBJECT_MANAGER.RemoveObject(NativeInfo::ObjectInfo(Define::ObjectType_PLAYER, gameClientList[oid]->GetPlayerOID()));

	this->gameClientList[oid]->Stop();
	this->gameClientList.erase(oid);

	if (this->gameClientList.empty())
		Shutdown();
}

bool DummyClientManager::IsMyPlayer(std::string_view characterName)
{
	READ_LOCK(this->gameMutex);

	auto iter_begin = this->gameClientList.begin();
	auto iter_end = this->gameClientList.end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		if (IS_SAME(characterName, (*iter_begin).second->GetCharacterName()))
			return true;
	}

	return false;
}

void DummyClientManager::Shutdown(void)
{
	std::cout << "\n" << std::endl;
	CORE_ALL_LOG(LogType::LOG_ERROR, "Cannot Connect, Shutdown");
	Sleep(1000);
	abort();
}
