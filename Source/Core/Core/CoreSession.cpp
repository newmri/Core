#include "CoreInclude.h"

CoreSession::CoreSession(boost::asio::ip::tcp::socket socket, const size_t uid) : socket(std::move(socket)), uid(uid)
{

}

CoreSession::CoreSession(boost::asio::io_context& ioContext) : socket(ioContext)
{

}

CoreSession::~CoreSession()
{

}

size_t CoreSession::GetUID(void)
{
	return this->uid;
}

void CoreSession::SetUID(const size_t uid)
{
	this->uid = uid;
}

boost::asio::ip::tcp::socket& CoreSession::GetSocket(void)
{
	return this->socket;
}

bool CoreSession::IsConnected(void)
{
	return this->socket.is_open();
}