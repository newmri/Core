#pragma once

class CoreServerSession : public CoreSession
{
public:
	CoreServerSession(boost::asio::io_context& ioContext);
	virtual ~CoreServerSession();

public:
	void Connect(const boost::asio::ip::tcp::resolver::results_type& endpoint);
	void Close(void);

public:
	virtual void Write(const CorePacket& packet);

private:
	virtual void Write(void);
	virtual void ReadHeader(void);
	virtual void ReadBody(void);

protected:
	void ProcessPacket(const uint8_t* data, size_t size);

private:
	boost::asio::io_context& ioContext;
};