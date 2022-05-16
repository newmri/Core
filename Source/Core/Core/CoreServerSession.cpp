#include "CoreInclude.h"

CoreServerSession::CoreServerSession(const int64_t oid, boost::asio::io_context& ioContext) :
	ioContext(ioContext),
	CoreSession(oid, ioContext)
{
}

CoreServerSession::~CoreServerSession()
{
	Close(true);
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

				this->onConnectedFunc(shared_from_this());

				ReadHeader();
			}
		});
}

void CoreServerSession::Close(bool isForce)
{
	boost::asio::post(this->ioContext, [this, isForce]()
		{
			if (this->socket.is_open())
			{
				this->ioContext.stop();
				this->socket.close();

				if(!isForce)
					this->onDisconnectedFunc(shared_from_this());
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
				this->processPacketFunc(shared_from_this(), this->read.GetBody(), this->read.GetBodySize());
				ReadHeader();
			}
		});
}

void CoreServerSession::SetOnConnectedFunc(std::function<void(std::shared_ptr<CoreServerSession>)> func)
{
	this->onConnectedFunc = func;
}

void CoreServerSession::SetOnDisconnectedFunc(std::function<void(std::shared_ptr<CoreServerSession>)> func)
{
	this->onDisconnectedFunc = func;
}

void CoreServerSession::SetProcessPacketFunc(std::function<void(std::shared_ptr<CoreServerSession>, uint8_t*, uint32_t)> func)
{
	this->processPacketFunc = func;
}

std::string_view CoreServerSession::GetAccountID(void)
{
	return this->accountID;
}


std::string_view CoreServerSession::GetAccountPassword(void)
{
	return this->accountPassword;
}

int64_t CoreServerSession::GetCharacterUID(void)
{
	return this->characterUID;
}

void CoreServerSession::SetAccountID(std::string_view accountID)
{
	this->accountID = accountID;
}

void CoreServerSession::SetAccountPassword(std::string_view accountPassword)
{
	this->accountPassword = accountPassword;
}

void CoreServerSession::SetCharacterUID(int64_t characterUID)
{
	this->characterUID = characterUID;
}

int32_t CoreServerSession::GetToken(void) const
{
	return this->token;
}

void CoreServerSession::SetToken(const int32_t token)
{
	this->token = token;
}