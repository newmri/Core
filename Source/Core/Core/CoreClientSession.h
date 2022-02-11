#pragma once

class CoreServer;

class CoreClientSession : public CoreSession, public std::enable_shared_from_this<CoreClientSession>
{
public:
	CoreClientSession(boost::asio::ip::tcp::socket socket, const size_t oid, CoreServer* server);
	virtual ~CoreClientSession() override;

public:
	void Start(void);
	virtual void Write(const CorePacket& packet) override;

private:
	virtual void Write(void) override;
	virtual void ReadHeader(void) override;
	virtual void ReadBody(void) override;

private:
	bool IsValidPacketSpeed(void);

private:
	CoreServer* server;

private:
	TIME_VALUE speedHackCheckTime = SEC;
	TIME_VALUE prevPacketTime = 0;
	size_t packetCount = 0;
	size_t maxPacketCount = 10000;
};