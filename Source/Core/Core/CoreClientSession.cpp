#include "CoreInclude.h"

CoreClientSession::CoreClientSession(const int64_t oid, boost::asio::ip::tcp::socket socket, CoreServer* server) :
	CoreSession(oid, std::move(socket)), server(server)
{

}

CoreClientSession::~CoreClientSession()
{

}

void CoreClientSession::Start(void)
{
	ReadHeader();
}

void CoreClientSession::Write(const CorePacket& packet)
{
	this->writeQueue.push(packet);

	if (IS_SAME(1, this->writeQueue.size()))
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
				this->server->Close(self, false);
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
				this->server->Close(shared_from_this(), false);
			}
			else
			{
				if (IsValidPacketSpeed() && this->read.DecodeHeader())
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
				this->server->Close(shared_from_this(), false);
			}
			else
			{
				this->server->ProcessPacket(shared_from_this(), this->read.GetBody(), this->read.GetBodySize());
				ReadHeader();
			}
		});
}

bool CoreClientSession::IsValidPacketSpeed(void)
{
	if (IS_SAME(0, this->prevPacketTime))
		this->prevPacketTime = CORE_TIME_MANAGER.GetNowMilliSeconds();
	else
	{
		TIME_VALUE now = CORE_TIME_MANAGER.GetNowMilliSeconds();
		TIME_VALUE timeDiff = now - this->prevPacketTime;

		if (this->speedHackCheckTime < timeDiff)
		{
			this->prevPacketTime = now;
			this->packetCount = 0;
		}
		else
		{
			++this->packetCount;
		}

		if (this->maxPacketCount <= this->packetCount)
		{
			CORE_LOG.Log(LogType::LOG_HACK, GetOID(), ENUM_TO_STR(SPEED_HACK));
			this->server->Close(shared_from_this());
			return false;
		}
	}

	return true;
}

void CoreClientSession::CheckPingPongTime(void)
{
	if (IS_SAME(0, this->prevPingPongTime))
	{
		UpdatePingPongTime();
		this->server->SendPing(shared_from_this());
		return;
	}

	TIME_VALUE now = CORE_TIME_MANAGER.GetNowMilliSeconds();
	TIME_VALUE timeDiff = now - this->prevPingPongTime;

	if (this->pingPongCheckTime < timeDiff)
	{
		server->Close(shared_from_this());
		return;
	}

	this->server->SendPing(shared_from_this());
}

void CoreClientSession::UpdatePingPongTime(void)
{
	this->prevPingPongTime = CORE_TIME_MANAGER.GetNowMilliSeconds();
}