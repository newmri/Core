#include "Include.h"
#include <boost/format.hpp>

LoginClient::LoginClient(const int64_t uid) : CoreClient(uid)
{
	this->handler = new LoginPacketHandler;

	MakeAccountInfo();
}

LoginClient::~LoginClient()
{
	SAFE_DELETE(this->handler);
}

void LoginClient::MakeAccountInfo(void)
{
	boost::format format("%04d\n");
	format% GetUID();
	this->id = "test" + format.str();
	this->password = this->id;
}

void LoginClient::Connect(void)
{
	auto client = CoreClient::downcasted_shared_from_this<LoginClient>();

	WEB_MANAGER.Signup(client);
	if (WEB_MANAGER.Login(client))
	{
		auto connectInfo = WEB_MANAGER.SelectServer(Define::ServerType_Login);
		CoreClient::Connect(connectInfo.ServerIP, connectInfo.ServerPort);
		boost::asio::io_context& ioContext = GetContext();
		boost::thread t(boost::bind(&boost::asio::io_context::run, &ioContext));
	}
}

void LoginClient::OnConnected(void)
{
	PACKET_SEND_MANAGER.builder.Clear();
	auto message = LoginPacket::CreateCS_LOGIN_REQ(PACKET_SEND_MANAGER.builder, GetAccountUID(), GetToken());
	PACKET_SEND_MANAGER.Send(session, LoginPacket::Packet_CS_LOGIN_REQ, message.Union());
}

void LoginClient::OnDisconnected(void)
{
	DUMMY_CLIENT.DeleteLoginClient(GetUID());
}

void LoginClient::ProcessPacket(const uint8_t* data, size_t size)
{
	auto verifier = flatbuffers::Verifier(data, size);

	if (LoginPacket::VerifyRootBuffer(verifier))
	{
		auto root = LoginPacket::GetRoot(data);

		this->handler->Handle(this->session, root->packet_type(), root->packet());
	}
}

std::string_view LoginClient::GetID(void)
{
	return this->id;
}

std::string_view LoginClient::GetPassword(void)
{
	return this->password;
}

int64_t LoginClient::GetAccountUID(void) const
{
	return this->accountUID;
}

int32_t LoginClient::GetToken(void) const
{
	return this->token;
}

void LoginClient::SetAccountUID(const int64_t accountUID)
{
	this->accountUID = accountUID;
}

void LoginClient::SetToken(const int32_t token)
{
	this->token = token;
}
