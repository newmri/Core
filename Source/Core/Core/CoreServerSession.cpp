#include "CoreInclude.h"
#include "CoreServerSession.h"

CoreServerSession::CoreServerSession(boost::asio::io_context& ioContext) :
	ioContext(ioContext),
	CoreSession(ioContext)
{
}

CoreServerSession::~CoreServerSession()
{
	this->ioContext.stop();
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
			}
			else
			{
				ReadHeader();
			}
		});
}

void CoreServerSession::Close(void)
{
	boost::asio::post(this->ioContext, [this]() { this->socket.close(); });
}

void CoreServerSession::Write(const CorePacket& packet)
{
	boost::asio::post(this->ioContext,
		[this, packet]()
		{
			this->writeQueue.push(packet);

			if (!this->writeQueue.empty())
			{
				Write();
			}
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
				ProcessPacket(this->read.GetBody(), this->read.GetBodySize());
				ReadHeader();
			}
		});
}

void CoreServerSession::ProcessPacket(const uint8_t* data, size_t size)
{

}