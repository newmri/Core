#include "CoreInclude.h"

CoreClient::CoreClient() : resolver(ioContext), session(ioContext, this)
{

}

CoreClient::~CoreClient()
{

}

void CoreClient::Connect(std::string_view ip, std::string_view port)
{
	this->session.Connect(resolver.resolve(ip, port));
}

bool CoreClient::IsConnected(void)
{
	return this->session.IsConnected();
}

void CoreClient::Write(const CorePacket& packet)
{
	this->session.Write(packet);
}

boost::asio::io_context& CoreClient::GetContext(void)
{
	return this->ioContext;
}