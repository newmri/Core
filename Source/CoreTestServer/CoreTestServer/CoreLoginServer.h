#pragma once

class CoreLoginPacketHandler;

class CoreLoginServer : public CoreServer
{
public:
	CoreLoginServer(const unsigned short port);
	~CoreLoginServer();

public:
	virtual void ProcessPacket(std::shared_ptr<CoreClientSession> session, const uint8_t* data, size_t size) override;

private:
	CoreLoginPacketHandler* handler;
};