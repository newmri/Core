#pragma once

class CoreServer;

class CoreClientSession : public std::enable_shared_from_this<CoreClientSession>
{
public:
	CoreClientSession(boost::asio::ip::tcp::socket socket, CoreServer* server);
	~CoreClientSession();

public:
	boost::asio::ip::tcp::socket& GetSocket(void);

public:
	void Start(void);
	void Write(const CorePacket& packet);

private:
	void Write(void);
	void ReadHeader(void);
	void ReadBody(void);

private:
	boost::asio::ip::tcp::socket socket;
	CorePacket read;
	CoreQueue<CorePacket> writeQueue;

	CoreServer* server;
};