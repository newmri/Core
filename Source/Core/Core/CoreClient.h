#pragma once

class CoreClient
{
public:
	CoreClient(const int64_t oid);
	virtual ~CoreClient();

public:
	bool Connect(std::string_view ip, std::string_view port);
	void Stop(void);

public:
	bool IsConnected(void);
	
public:
	void Write(const CorePacket& packet);

public:
	boost::asio::io_context& GetContext(void);
	int64_t GetPlayerOID(void);

private:
	boost::asio::io_context ioContext;
	boost::asio::ip::tcp::resolver resolver;
	boost::thread thread;

protected:
	std::shared_ptr<CoreServerSession> session;
};