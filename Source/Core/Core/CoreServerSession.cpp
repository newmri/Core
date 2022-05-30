#include "CoreInclude.h"

CoreServerSession::CoreServerSession(const int64_t oid, boost::asio::io_context& ioContext) :
	ioContext(ioContext),
	CoreSession(oid, ioContext)
{
}

CoreServerSession::~CoreServerSession()
{
}

bool CoreServerSession::Connect(const boost::asio::ip::tcp::resolver::results_type& endpoint)
{
	boost::system::error_code error;
	boost::asio::connect(this->socket, endpoint, error);
	if (error)
	{
		CORE_LOG.Log(LogType::LOG_ERROR, "connect error " + error.value() + error.message());
		return false;
	}
	else
	{
		boost::asio::ip::tcp::no_delay option(true);
		this->socket.set_option(option);

		this->onConnectedFunc(shared_from_this());

		ReadHeader();
	}

	return true;
}

void CoreServerSession::Close(void)
{
	auto self(shared_from_this());

	CORE_TIME_DELEGATE_MANAGER.Push(
		CoreTimeDelegate<>(
			std::bind(&CoreServerSession::OnDisconnectedFunc, shared_from_this()),
			0));
}

void CoreServerSession::Write(const CorePacket& packet)
{
	this->writeQueue.push(packet);

	if (IS_SAME(1, this->writeQueue.size()))
		Write();
}

void CoreServerSession::Write(void)
{
	auto self(shared_from_this());

	CorePacket packet = this->writeQueue.front();

	boost::asio::async_write(this->socket,
		boost::asio::buffer(packet.GetData(), packet.GetSize()),
		[this, self](boost::system::error_code error, size_t)
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
	auto self(shared_from_this());

	boost::asio::async_read(this->socket,
		boost::asio::buffer(this->read.GetData(), CorePacket::HEADER_SIZE),
		[this, self](boost::system::error_code error, size_t)
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
	auto self(shared_from_this());

	boost::asio::async_read(this->socket,
		boost::asio::buffer(this->read.GetBody(), this->read.GetBodySize()),
		[this, self](boost::system::error_code error, size_t)
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
				this->processPacketFunc(self, this->read.GetBody(), this->read.GetBodySize());
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

void CoreServerSession::OnDisconnectedFunc(void)
{
	this->onDisconnectedFunc(shared_from_this());
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