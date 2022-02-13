#pragma once

struct ServerConfig
{
	std::string ServerType;
	int GroupID = 0;
	int WorldID = 0;
	int ServerID = 0;
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

protected:
	std::atomic<size_t> oid = 0;

protected:
	boost::asio::io_context ioContext;
	boost::asio::ip::tcp::acceptor acceptor;

	boost::shared_ptr<boost::asio::io_service::work> asyncWork;
	boost::thread_group asyncThread;

protected:
	std::shared_mutex mutex;
	std::map<size_t, std::shared_ptr<CoreClientSession>> sessionList;

protected:
	std::shared_ptr<ServerConfig> serverConfig;
};