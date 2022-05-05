#include "Include.h"

LoginClient::LoginClient()
{
	this->handler = new LoginPacketHandler;
}

LoginClient::~LoginClient()
{
	SAFE_DELETE(this->handler);
}

void LoginClient::OnConnected(void)
{
	for (int i = 0; i < 10; ++i)
	{
		PACKET_SEND_MANAGER.builder.Clear();
		auto message = LoginPacket::CreateCS_TEST_REQ(PACKET_SEND_MANAGER.builder, i);
		PACKET_SEND_MANAGER.Send(session, LoginPacket::Packet_CS_TEST_REQ, message.Union());
	}
}

void LoginClient::OnDisconnected(void)
{
	DUMMY_CLIENT.DeleteLoginClient(CoreClient::downcasted_shared_from_this<LoginClient>());
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