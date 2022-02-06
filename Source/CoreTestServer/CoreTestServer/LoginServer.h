#pragma once

class LoginPacketHandler;

class LoginServer : public CoreServer
{
public:
	LoginServer(const unsigned short port);
	virtual ~LoginServer() override;

public:
	virtual void ProcessPacket(std::shared_ptr<CoreClientSession> session, const uint8_t* data, size_t size) override;

private:
	LoginPacketHandler* handler;
};