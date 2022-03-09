#include "Include.h"

GameServer::GameServer(const unsigned short port) : CoreServer(port)
{
	this->handler = new GamePacketHandler;
}

GameServer::~GameServer()
{
	SAFE_DELETE(this->handler);
}

void GameServer::Run(void)
{
	CoreServer::Run();
}

void GameServer::Stop(void)
{
	CoreServer::Stop();
}

void GameServer::ProcessPacket(std::shared_ptr<CoreClientSession> session, const uint8_t* data, size_t size)
{
	auto verifier = flatbuffers::Verifier(data, size);

	if (GamePacket::VerifyRootBuffer(verifier))
	{
		auto root = GamePacket::GetRoot(data);

		this->handler->Handle(session, root->packet_type(), root->packet());
	}
}

void GameServer::SendPing(std::shared_ptr<CoreClientSession> session)
{
	GamePacketFunc::SC_PING_REQ(session);
}