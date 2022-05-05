#pragma once

class CoreClient : public inheritable_enable_shared_from_this<CoreClient>
{
public:
	CoreClient();
	virtual ~CoreClient();

public:
	void Connect(std::string_view ip, std::string_view port);

public:
	bool IsConnected(void);
	
public:
	void Write(const CorePacket& packet);

public:
	boost::asio::io_context& GetContext(void);

public:
	virtual void OnConnected(void) CORE_PURE;
	virtual void OnDisconnected(void) CORE_PURE;
	virtual void ProcessPacket(const uint8_t* data, size_t size) CORE_PURE;

protected:
	boost::asio::io_context ioContext;
	boost::asio::ip::tcp::resolver resolver;
	CoreServerSession session;
};