#pragma once

class CoreSession
{

public:
	CoreSession(const int64_t oid, boost::asio::ip::tcp::socket socket);
	CoreSession(const int64_t oid, boost::asio::io_context& ioContext);
	virtual ~CoreSession();

public:
	int64_t GetOID(void);
	int64_t GetAccountUID(void);
	int64_t GetPlayerOID(void) const;

public:
	void SetOID(const int64_t oid);
	void SetAccountUID(const int64_t accountUID);
	void SetPlayerOID(const int64_t playerOID);

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
	int64_t playerOID = 0;

protected:
	boost::asio::ip::tcp::socket socket;
	CorePacket read;
	CoreQueue<CorePacket> writeQueue;
	std::atomic<bool> isWriting = false;
};