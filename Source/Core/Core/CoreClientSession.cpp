#include "CoreInclude.h"

CoreClientSession::CoreClientSession(boost::asio::ip::tcp::socket socket, CoreServer* server) : socket(std::move(socket)), server(server)
{

}

CoreClientSession::~CoreClientSession()
{

}

boost::asio::ip::tcp::socket& CoreClientSession::GetSocket(void)
{
	return this->socket;
}

void CoreClientSession::Start(void)
{
	ReadHeader();
}

void CoreClientSession::Write(const CorePacket& packet)
{
	this->writeQueue.push(packet);

	if (!this->writeQueue.empty())
		Write();
}

void CoreClientSession::Write(void)
{
	auto self(shared_from_this());

	CorePacket packet = this->writeQueue.front();

	boost::asio::async_write(this->socket,
		boost::asio::buffer(packet.GetData(), packet.GetSize()),
		[this, self](boost::system::error_code error, size_t)
		{
			if (error)
			{
				this->server->Close(shared_from_this());
			}
			else
			{
				this->writeQueue.pop();
				if (!this->writeQueue.empty())
					Write();
			}
		});
}

void CoreClientSession::ReadHeader(void)
{
	auto self(shared_from_this());

	boost::asio::async_read(this->socket,
		boost::asio::buffer(this->read.GetData(), CorePacket::HEADER_SIZE),
		[this, self](boost::system::error_code error, size_t)
		{
			if (error)
			{
				this->server->Close(shared_from_this());
			}
			else if(this->read.DecodeHeader())
			{
				ReadBody();
			}
		});
}

void CoreClientSession::ReadBody(void)
{
	auto self(shared_from_this());

	boost::asio::async_read(this->socket,
		boost::asio::buffer(this->read.GetBody(),this->read.GetBodySize()),
		[this, self](boost::system::error_code error, size_t)
		{
			if (error)
			{
				if (IS_SAME(boost::asio::error::eof, error))
					CORE_LOG.Log("Disconnected");
				else
					CORE_LOG.Log(LogType::LOG_ERROR, "read error " + error.value() + error.message());

				this->server->Close(shared_from_this());
			}
			else
			{
				this->server->ProcessPacket(shared_from_this(), this->read.GetBody(), this->read.GetBodySize());
				ReadHeader();
			}
		});
}