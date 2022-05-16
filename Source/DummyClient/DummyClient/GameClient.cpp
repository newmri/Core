#include "Include.h"

GameClient::GameClient(std::shared_ptr<CoreServerSession> session, const int64_t characterUID) : CoreClient(session->GetOID())
{
	this->session->SetOnConnectedFunc(std::bind(&DummyClientManager::OnGameServerConnected, &DUMMY_CLIENT, std::placeholders::_1));
	this->session->SetOnDisconnectedFunc(std::bind(&DummyClientManager::OnGameServerDisconnected, &DUMMY_CLIENT, std::placeholders::_1));
	this->session->SetProcessPacketFunc(std::bind(&DummyClientManager::GameServerProcessPacket, &DUMMY_CLIENT, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	this->session->SetAccountUID(session->GetAccountUID());
	this->session->SetToken(session->GetToken());
	this->session->SetAccountID(session->GetAccountID());
	this->session->SetAccountPassword(session->GetAccountPassword());
	this->session->SetCharacterUID(characterUID);
}

GameClient::~GameClient()
{
	
}

void GameClient::Connect(void)
{
	auto connectInfo = WEB_MANAGER.SelectServer(Define::ServerType_Game);
	CoreClient::Connect(connectInfo.ServerIP, connectInfo.ServerPort);
}
