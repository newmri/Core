#pragma once

class CoreServer
{
public:
	CoreServer(const unsigned short port);

public:
	void Run(void);
	void Stop(void);

private:
	void Accept(void);

public:
	void Close(std::shared_ptr<CoreClientSession> session);

public:
	void ProcessPacket(std::shared_ptr<CoreClientSession> session, const uint8_t* data, size_t size);

private:
	boost::asio::io_context ioContext;
	boost::asio::ip::tcp::acceptor acceptor;

	boost::shared_ptr<boost::asio::io_service::work> asyncWork;
	boost::thread_group asyncThread;

	CoreList<std::shared_ptr<CoreClientSession>> sessionList;
};