#include "Include.h"
#include <boost/format.hpp>


LoginClient::LoginClient(const int64_t uid) : CoreClient(uid)
{
	this->handler = new LoginPacketHandler;
}

LoginClient::~LoginClient()
{
	SAFE_DELETE(this->handler);
}

void LoginClient::OnConnected(void)
{
	Signup();
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

void LoginClient::Signup(void)
{
	boost::format format("%04d\n");
	format% GetUID();
	std::string id = "test" + format.str();

	WEB_MANAGER.Signup(id, id);
}
