#pragma once

class GamePacketHandler;

class GameServer : public CoreServer
{
public:
	GameServer(const unsigned short port);
	virtual ~GameServer() override;

public:
	virtual void Run(void) override;
	virtual void Stop(void) override;

public:
	virtual void ProcessPacket(std::shared_ptr<CoreClientSession> session, const uint8_t* data, size_t size) override;
	virtual void SendPing(std::shared_ptr<CoreClientSession> session) override;

private:
	GamePacketHandler* handler;
};