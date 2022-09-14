#include "CoreInclude.h"

CoreClientSession::CoreClientSession(const int64_t oid, boost::asio::ip::tcp::socket socket, CoreServer* server) :
	CoreSession(oid, std::move(socket)), server(server)
{

}

CoreClientSession::~CoreClientSession()
{
	Close();
}

void CoreClientSession::Start(void)
{
	ReadHeader();
}

void CoreClientSession::Close(void)
{
	CoreSession::Close();
}

void CoreClientSession::Write(const CorePacket& packet)
{
	this->writeQueue.push(packet);

	if (!this->isWriting)
	{
		this->isWriting = true;
		Write();
	}
}

void CoreClientSession::Write(void)
{
	if (!IsConnected())
	{
		this->writeQueue.clear();
		return;
	}

	auto self(shared_from_this());

	CorePacket packet = this->writeQueue.front();

	boost::asio::async_write(this->socket,
		boost::asio::buffer(packet.GetData(), packet.GetSize()),
		[this, self](boost::system::error_code error, size_t)
		{
			if (error)
			{
				this->server->Close(self);
			}
			else
			{
				this->writeQueue.pop();
				if (this->writeQueue.empty())
					this->isWriting = false;
				else
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
				this->server->Close(self);
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
				this->server->Close(self);
			}
			else
			{
				this->server->ProcessPacket(self, this->read.GetBody(), this->read.GetBodySize());
				ReadHeader();
			}
		});
}

bool CoreClientSession::IsValidPacketSpeed(void)
{
#if _DEBUG
	return true;
#endif // _DEBUG

	TIME_VALUE _prevPacketTime = 0;
	{
		READ_LOCK(this->speedHackMutex);
		_prevPacketTime = this->prevPingPongTime;
	}

	if (IS_SAME(0, _prevPacketTime))
	{
		WRITE_LOCK(this->speedHackMutex);
		this->prevPacketTime = CORE_TIME_MANAGER.GetNowMilliSeconds();
	}
	else
	{
		TIME_VALUE now = CORE_TIME_MANAGER.GetNowMilliSeconds();
		TIME_VALUE timeDiff = now - _prevPacketTime;

		int32_t _packetCount = 0;
		if (this->speedHackCheckTime < timeDiff)
		{
			WRITE_LOCK(this->speedHackMutex);
			this->prevPacketTime = now;
			this->packetCount = 0;
		}
		else
		{
			WRITE_LOCK(this->speedHackMutex);
			_packetCount = ++this->packetCount;
		}

		if (this->maxPacketCount <= _packetCount)
		{
			CORE_LOG.Log(LogType::LOG_HACK, GetOID(), ENUM_TO_STR(SPEED_HACK) + " PacketCount: " + TO_STR(_packetCount));
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
		CORE_LOG.Log(LogType::LOG_DEBUG, GetOID(), ENUM_TO_STR(PING_PONG_NO_RESPONSE) + " TimeDiff: " + TO_STR(timeDiff));
		server->Close(shared_from_this());
		return;
	}

	this->server->SendPing(shared_from_this());
}

void CoreClientSession::UpdatePingPongTime(void)
{
	this->prevPingPongTime = CORE_TIME_MANAGER.GetNowMilliSeconds();
}