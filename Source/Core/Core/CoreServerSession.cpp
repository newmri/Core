#include "CoreInclude.h"

CoreServerSession::CoreServerSession(boost::asio::io_context& ioContext, CoreClient* client) :
	ioContext(ioContext),
	CoreSession(ioContext),
	client(client)
{
}

CoreServerSession::~CoreServerSession()
{
	Close();
}

void CoreServerSession::Connect(const boost::asio::ip::tcp::resolver::results_type& endpoint)
{
	boost::asio::async_connect(this->socket, endpoint,
		[this](boost::system::error_code error, boost::asio::ip::tcp::endpoint)
		{
			if (error)
			{
				CORE_LOG.Log(LogType::LOG_ERROR, "connect error " + error.value() + error.message());
				Close();
			}
			else
			{
				boost::asio::ip::tcp::no_delay option(true);
				this->socket.set_option(option);

				this->client->OnConnected();

				ReadHeader();
			}
		});
}

void CoreServerSession::Close(void)
{
	boost::asio::post(this->ioContext, [this]()
		{
			if (this->socket.is_open())
			{
				this->ioContext.stop();
				this->socket.close();
				this->client->OnDisconnected();
			}
		});
}

void CoreServerSession::Write(const CorePacket& packet)
{
	boost::asio::post(this->ioContext,
		[this, packet]()
		{
			this->writeQueue.push(packet);

			if (IS_SAME(1, this->writeQueue.size()))
				Write();
		});
}

void CoreServerSession::Write(void)
{
	CorePacket packet = this->writeQueue.front();

	boost::asio::async_write(this->socket,
		boost::asio::buffer(packet.GetData(), packet.GetSize()),
		[this](boost::system::error_code error, size_t)
		{
			if (error)
			{
				Close();
			}
			else
			{
				this->writeQueue.pop();
				if (!this->writeQueue.empty())
					Write();
			}
		});
}

void CoreServerSession::ReadHeader(void)
{
	boost::asio::async_read(this->socket,
		boost::asio::buffer(this->read.GetData(), CorePacket::HEADER_SIZE),
		[this](boost::system::error_code error, size_t)
		{
			if (error)
			{
				if (IS_SAME(boost::asio::error::eof, error))
					CORE_LOG.Log("Disconnected");

				else
					CORE_LOG.Log(LogType::LOG_ERROR, "read error " + error.value() + error.message());

				Close();
			}
			else if (this->read.DecodeHeader())
			{
				ReadBody();
			}
		});
}

void CoreServerSession::ReadBody(void)
{
	boost::asio::async_read(this->socket,
		boost::asio::buffer(this->read.GetBody(), this->read.GetBodySize()),
		[this](boost::system::error_code error, size_t)
		{
			if (error)
			{
				if (IS_SAME(boost::asio::error::eof, error))
					CORE_LOG.Log("Disconnected");
				else
					CORE_LOG.Log(LogType::LOG_ERROR, "read error " + error.value() + error.message());

				Close();
			}
			else
			{
				this->client->ProcessPacket(this->read.GetBody(), this->read.GetBodySize());
				ReadHeader();
			}
		});
}