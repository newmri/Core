#pragma once

class CoreServer;

class CoreClientSession : public CoreSession, public std::enable_shared_from_this<CoreClientSession>
{
public:
	CoreClientSession(boost::asio::ip::tcp::socket socket, CoreServer* server);
	virtual ~CoreClientSession() override;

public:
	void Start(void);
	virtual void Write(const CorePacket& packet) override;

private:
	virtual void Write(void) override;
	virtual void ReadHeader(void) override;
	virtual void ReadBody(void) override;

private:
	CoreServer* server;
};