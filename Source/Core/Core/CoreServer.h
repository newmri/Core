#pragma once

struct ServerInfo
{
	uint8_t groupID = 0;
	int32_t serverPort = 0;
};

enum class CountryCode : uint8_t
{
	None = 0,
	KR
};

struct ServerConfig
{
	std::string serverType;
	CountryCode countryCode = CountryCode::None;
	uint8_t worldID = 0;
	uint8_t serverID = 0;
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

	std::shared_ptr<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> asyncWork;
	boost::thread_group asyncThread;

protected:
	std::shared_mutex mutex;
	std::unordered_map<int64_t, std::shared_ptr<CoreClientSession>> sessionList;
};