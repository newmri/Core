#pragma once

class CoreClient
{
public:
	CoreClient(const int64_t oid);
	virtual ~CoreClient();

public:
	bool Connect(std::string_view ip, std::string_view port);

public:
	bool IsConnected(void);
	
public:
	void Write(const CorePacket& packet);

public:
	boost::asio::io_context& GetContext(void);

private:
	boost::asio::io_context ioContext;
	boost::asio::ip::tcp::resolver resolver;
	boost::thread_group asyncThread;

protected:
	std::shared_ptr<CoreServerSession> session;
};