#include "Include.h"

LoginServer::LoginServer(const unsigned short port) : CoreServer(port)
{
	this->handler = new LoginPacketHandler;
}

LoginServer::~LoginServer()
{
	SAFE_DELETE(this->handler);
}

void LoginServer::Run(void)
{
	CoreServer::Run();
}

void LoginServer::Stop(void)
{
	CoreServer::Stop();
}

void LoginServer::ProcessPacket(std::shared_ptr<CoreClientSession> session, const uint8_t* data, size_t size)
{
	auto verifier = flatbuffers::Verifier(data, size);

	if (Login::VerifyRootBuffer(verifier))
	{
		auto root = Login::GetRoot(data);

		this->handler->Handle(session, root->packet_type(), root->packet());
	}
}

void LoginServer::SendPing(std::shared_ptr<CoreClientSession> session)
{
	LoginPacketFunc::SC_PING_REQ(session);
}