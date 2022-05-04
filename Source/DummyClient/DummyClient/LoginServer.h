#pragma once

class LoginPacketHandler;

class LoginServer : public CoreServer
{
public:
	LoginServer(const unsigned short port);
	virtual ~LoginServer() override;

public:
	virtual void Run(void) override;
	virtual void Stop(void) override;

public:
	virtual void ProcessPacket(std::shared_ptr<CoreClientSession> session, const uint8_t* data, size_t size) override;
	virtual void SendPing(std::shared_ptr<CoreClientSession> session) override;

private:
	LoginPacketHandler* handler;
};