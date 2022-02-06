#pragma once

class CoreSession
{

public:
	CoreSession(boost::asio::ip::tcp::socket socket);
	CoreSession(boost::asio::io_context& ioContext);
	virtual ~CoreSession();

public:
	boost::asio::ip::tcp::socket& GetSocket(void);
	bool IsConnected(void);

public:
	virtual void Write(const CorePacket& packet) CORE_PURE;

protected:
	virtual void Write(void) CORE_PURE;
	virtual void ReadHeader(void) CORE_PURE;
	virtual void ReadBody(void) CORE_PURE;

protected:
	boost::asio::ip::tcp::socket socket;
	CorePacket read;
	CoreQueue<CorePacket> writeQueue;
};