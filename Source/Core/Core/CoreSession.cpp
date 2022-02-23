#include "CoreInclude.h"

CoreSession::CoreSession(boost::asio::ip::tcp::socket socket, const int64_t oid) : socket(std::move(socket)), oid(oid)
{

}

CoreSession::CoreSession(boost::asio::io_context& ioContext) : socket(ioContext)
{

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

int64_t CoreSession::GetAccountID(void)
{
	return this->accountID;
}

void CoreSession::SetAccountID(const int64_t accountID)
{
	this->accountID = accountID;
}

boost::asio::ip::tcp::socket& CoreSession::GetSocket(void)
{
	return this->socket;
}

bool CoreSession::IsConnected(void)
{
	return this->socket.is_open();
}