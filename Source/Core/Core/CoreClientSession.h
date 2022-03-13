#pragma once

class CoreServer;

class CoreClientSession : public CoreSession, public std::enable_shared_from_this<CoreClientSession>
{
public:
	CoreClientSession(boost::asio::ip::tcp::socket socket, const int64_t oid, CoreServer* server);
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

public:
	void CheckPingPongTime(void);
	void UpdatePingPongTime(void);

public:
	void SetPlayerUID(const int64_t playerUID);
	int64_t GetPlayerUID(void) const;

private:
	CoreServer* server;

private:
	TIME_VALUE speedHackCheckTime = SEC;
	TIME_VALUE prevPacketTime = 0;
	size_t packetCount = 0;
	size_t maxPacketCount = 10;

private:
	TIME_VALUE pingPongCheckTime = MIN;
	TIME_VALUE prevPingPongTime = 0;

private:
	int64_t playerUID = 0;
};