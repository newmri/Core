#include "CoreInclude.h"

CoreSession::CoreSession(const int64_t oid, boost::asio::ip::tcp::socket socket) : oid(oid), socket(std::move(socket))
{
	boost::asio::ip::tcp::no_delay option(true);
	this->socket.set_option(option);
}

CoreSession::CoreSession(const int64_t oid, boost::asio::io_context& ioContext) : oid(oid), socket(ioContext)
{

}

CoreSession::~CoreSession()
{

}

int64_t CoreSession::GetOID(void)
{
	return this->oid;
}

int64_t CoreSession::GetAccountUID(void)
{
	return this->accountUID;
}

int64_t CoreSession::GetPlayerOID(void) const
{
	return this->playerOID;
}

void CoreSession::SetOID(const int64_t oid)
{
	this->oid = oid;
}

void CoreSession::SetAccountUID(const int64_t accountUID)
{
	this->accountUID = accountUID;
}

void CoreSession::SetPlayerOID(const int64_t playerOID)
{
	this->playerOID = playerOID;
}

boost::asio::ip::tcp::socket& CoreSession::GetSocket(void)
{
	return this->socket;
}

bool CoreSession::IsConnected(void)
{
	return this->socket.is_open();
}