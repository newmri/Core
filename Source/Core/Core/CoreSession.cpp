#include "CoreInclude.h"

CoreSession::CoreSession(boost::asio::ip::tcp::socket socket, const int64_t oid) : socket(std::move(socket)), oid(oid)
{
	boost::asio::ip::tcp::no_delay option(true);
	this->socket.set_option(option);
}

CoreSession::CoreSession(boost::asio::io_context& ioContext) : socket(ioContext)
{
	boost::asio::ip::tcp::no_delay option(true);
	this->socket.set_option(option);
}

CoreSession::~CoreSession()
{

}

int64_t CoreSession::GetOID(void)
{
	return this->oid;
}

void CoreSession::SetOID(const int64_t oid)
{
	this->oid = oid;
}

int64_t CoreSession::GetAccountUID(void)
{
	return this->accountUID;
}

void CoreSession::SetAccountUID(const int64_t accountUID)
{
	this->accountUID = accountUID;
}

boost::asio::ip::tcp::socket& CoreSession::GetSocket(void)
{
	return this->socket;
}

bool CoreSession::IsConnected(void)
{
	return this->socket.is_open();
}