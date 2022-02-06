#include "CoreInclude.h"

CoreSession::CoreSession(boost::asio::ip::tcp::socket socket) : socket(std::move(socket))
{

}

CoreSession::CoreSession(boost::asio::io_context& ioContext) : socket(ioContext)
{

}

CoreSession::~CoreSession()
{

}

boost::asio::ip::tcp::socket& CoreSession::GetSocket(void)
{
	return this->socket;
}

bool CoreSession::IsConnected(void)
{
	return this->socket.is_open();
}