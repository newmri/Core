#pragma once

struct ServerInfo
{
	int32_t GroupID = 0;
	int32_t ServerPort = 0;
};

struct ServerConfig
{
	std::string ServerType;
	int32_t WorldID = 0;
	int32_t ServerID = 0;
};

struct ServerConnectInfo
{
	std::string ServerIP;
	std::string ServerPort;
};

class CoreServer
{
public:
	CoreServer(const unsigned short port);
	virtual ~CoreServer();

public:
	virtual void Run(void);
	virtual void Stop(void);

private:
	void Accept(void);

public:
	void Close(std::shared_ptr<CoreClientSession> session);

public:
	virtual void ProcessPacket(std::shared_ptr<CoreClientSession> session, const uint8_t* data, size_t size) CORE_PURE;
	virtual void SendPing(std::shared_ptr<CoreClientSession> session) CORE_PURE;

private:
	bool IsValidSession(const int64_t oid);

protected:
	std::atomic<int64_t> oid = 0;

protected:
	boost::asio::io_context ioContext;
	boost::asio::ip::tcp::acceptor acceptor;

	boost::shared_ptr<boost::asio::io_service::work> asyncWork;
	boost::thread_group asyncThread;

protected:
	std::shared_mutex mutex;
	std::map<int64_t, std::shared_ptr<CoreClientSession>> sessionList;
};