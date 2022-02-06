#pragma once

class CoreServer;

class CoreClientSession : public CoreSession, public std::enable_shared_from_this<CoreClientSession>
{
public:
	CoreClientSession(boost::asio::ip::tcp::socket socket, CoreServer* server);
	virtual ~CoreClientSession();

public:
	void Start(void);
	virtual void Write(const CorePacket& packet);

private:
	virtual void Write(void);
	virtual void ReadHeader(void);
	virtual void ReadBody(void);

private:
	CoreServer* server;
};