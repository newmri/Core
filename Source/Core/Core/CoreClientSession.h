#pragma once

class CoreServer;

class CoreClientSession : public CoreSession, public std::enable_shared_from_this<CoreClientSession>
{
public:
	CoreClientSession(const int64_t oid, boost::asio::ip::tcp::socket socket, CoreServer* server);
	virtual ~CoreClientSession() override;

public:
	void Start(void);
	virtual void Close(void) override;
	virtual void Write(const CorePacket& packet) override;

private:
	virtual void Write(void) override;
	virtual void ReadHeader(void) override;
	virtual void ReadBody(void) override;

private:
	bool IsValidPacketSpeed(void);

public:
	void CheckPingPongTime(void);
	void UpdatePingPongTime(void);

private:
	CoreServer* server;

private:
	CACHE_ALIGN std::shared_mutex speedHackMutex;
	TIME_VALUE speedHackCheckTime = SEC;
	TIME_VALUE prevPacketTime = 0;
	int32_t packetCount = 0;
	int32_t maxPacketCount = 20;

private:
	TIME_VALUE pingPongCheckTime = MIN;
	TIME_VALUE prevPingPongTime = 0;
};