#include "Include.h"
#include <boost/format.hpp>

LoginClient::LoginClient(const int64_t oid) : CoreClient(oid)
{
	this->session->SetOnConnectedFunc(std::bind(&DummyClientManager::OnLoginServerConnected, &DUMMY_CLIENT, std::placeholders::_1));
	this->session->SetOnDisconnectedFunc(std::bind(&DummyClientManager::OnLoginServerDisconnected, &DUMMY_CLIENT, std::placeholders::_1));
	this->session->SetProcessPacketFunc(std::bind(&DummyClientManager::LoginServerProcessPacket, &DUMMY_CLIENT, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	MakeAccountInfo();
}

LoginClient::~LoginClient()
{
	
}

void LoginClient::MakeAccountInfo(void)
{
	boost::format format("%04d");
	format% this->session->GetOID();
	this->session->SetAccountID("test" + format.str());
	this->session->SetAccountPassword(this->session->GetAccountID());
}

bool LoginClient::Connect(void)
{
	WEB_MANAGER.Signup(this->session);
	if (WEB_MANAGER.Login(this->session))
	{
		auto connectInfo = WEB_MANAGER.SelectServer(Define::ServerType_Login);
		return CoreClient::Connect(connectInfo.ServerIP, connectInfo.ServerPort);
	}

	return false;
}

void LoginClient::SendLogout(void)
{
	LOGIN_PACKET_SEND_MANAGER.Clear();
	auto message = LoginPacket::CreateCS_LOGOUT_NOTI(LOGIN_PACKET_SEND_MANAGER.builder);
	LOGIN_PACKET_SEND_MANAGER.Send(this->session, LoginPacket::Packet_CS_LOGOUT_NOTI, message.Union());
}
