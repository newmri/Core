#pragma once

class CoreClient;

class CoreServerSession : public CoreSession
{
public:
	CoreServerSession(boost::asio::io_context& ioContext, CoreClient* client);
	virtual ~CoreServerSession() override;

public:
	void Connect(const boost::asio::ip::tcp::resolver::results_type& endpoint);
	void Close(void);

public:
	virtual void Write(const CorePacket& packet) override;

private:
	virtual void Write(void) override;
	virtual void ReadHeader(void) override;
	virtual void ReadBody(void) override;

private:
	boost::asio::io_context& ioContext;

private:
	CoreClient* client;
};