#pragma once

class CoreSession
{

public:
	CoreSession(const int64_t oid, boost::asio::ip::tcp::socket socket);
	CoreSession(const int64_t oid, boost::asio::io_context& ioContext);
	virtual ~CoreSession();

public:
	int64_t GetOID(void);
	void SetOID(const int64_t oid);

public:
	int64_t GetAccountUID(void);
	void SetAccountUID(const int64_t accountUID);

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
	int64_t oid = 0;
	int64_t accountUID = 0;

protected:
	boost::asio::ip::tcp::socket socket;
	CorePacket read;
	CoreQueue<CorePacket> writeQueue;
};