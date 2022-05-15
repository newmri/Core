#include "Include.h"

GameClient::GameClient(const int64_t oid, const int64_t accountUID, const int32_t token, const int64_t characterUID) : CoreClient(oid), characterUID(characterUID)
{
	SetAccountUID(accountUID);
	SetToken(token);

	this->handler = new GamePacketHandler;
}

GameClient::~GameClient()
{
	SAFE_DELETE(this->handler);
}

void GameClient::Connect(void)
{
	auto connectInfo = WEB_MANAGER.SelectServer(Define::ServerType_Game);
	CoreClient::Connect(connectInfo.ServerIP, connectInfo.ServerPort);
	boost::asio::io_context& ioContext = GetContext();
	boost::thread t(boost::bind(&boost::asio::io_context::run, &ioContext));
}

void GameClient::OnConnected(void)
{
	GAME_PACKET_SEND_MANAGER.Clear();
	auto message = GamePacket::CreateCS_LOGIN_REQ(GAME_PACKET_SEND_MANAGER.builder, session.GetAccountUID(), GetCharacterUID(), session.GetToken());
	GAME_PACKET_SEND_MANAGER.Send(session, GamePacket::Packet_CS_LOGIN_REQ, message.Union());
}

void GameClient::OnDisconnected(void)
{
	DUMMY_CLIENT.DeleteGameClient(this->session.GetOID());
}

void GameClient::ProcessPacket(const uint8_t* data, size_t size)
{
	auto verifier = flatbuffers::Verifier(data, size);

	if (GamePacket::VerifyRootBuffer(verifier))
	{
		auto root = GamePacket::GetRoot(data);

		this->handler->Handle(this->session, root->packet_type(), root->packet());
	}
}

int64_t GameClient::GetCharacterUID(void)
{
	return this->characterUID;
}
