#include "Include.h"

LoginServer::LoginServer(const unsigned short port) : CoreServer(port)
{
	this->handler = new LoginPacketHandler;
}

LoginServer::~LoginServer()
{
	SAFE_DELETE(this->handler);
}

void LoginServer::ProcessPacket(std::shared_ptr<CoreClientSession> session, const uint8_t* data, size_t size)
{
	auto verifier = flatbuffers::Verifier(data, size);

	if (Login::VerifyRootBuffer(verifier))
	{
		auto root = Login::GetRoot(data);

		this->handler->Handle(root->packet_type(), root->packet());

		// 임시
		// Handle안쪽에 session 넘기도록 수정 해야함

		flatbuffers::FlatBufferBuilder builder;

		auto data = Login::CreateSC_LOGIN_RES(builder, true);
		auto packet = Login::CreateRoot(builder, Login::Packet_SC_LOGIN_RES, data.Union());

		builder.Finish(packet);

		session->Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
	}
}
