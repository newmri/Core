#include "Include.h"

LoginClient::LoginClient()
{
	this->handler = new LoginPacketHandler;
}

LoginClient::~LoginClient()
{
	SAFE_DELETE(this->handler);
}

void LoginClient::ProcessPacket(const uint8_t* data, size_t size)
{
	auto verifier = flatbuffers::Verifier(data, size);

	if (Login::VerifyRootBuffer(verifier))
	{
		auto root = Login::GetRoot(data);

		// Handle안쪽에 session 넘기도록 수정 해야함
		this->handler->Handle(root->packet_type(), root->packet());
	}
}