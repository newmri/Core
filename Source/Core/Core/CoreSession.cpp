#include "CoreInclude.h"

CoreSession::CoreSession(boost::asio::ip::tcp::socket socket, const size_t oid) : socket(std::move(socket)), oid(oid)
{

}

CoreSession::CoreSession(boost::asio::io_context& ioContext) : socket(ioContext)
{

}

CoreSession::~CoreSession()
{

}

size_t CoreSession::GetOID(void)
{
	return this->oid;
}

void CoreSession::SetOID(const size_t oid)
{
	this->oid = oid;
}

boost::asio::ip::tcp::socket& CoreSession::GetSocket(void)
{
	return this->socket;
}

bool CoreSession::IsConnected(void)
{
	return this->socket.is_open();
}