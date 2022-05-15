#pragma once

class CoreClient;

class CoreServerSession : public CoreSession
{
public:
	CoreServerSession(const int64_t oid, boost::asio::io_context& ioContext, CoreClient* client);
	virtual ~CoreServerSession() override;

public:
	CoreClient* GetClient(void);

public:
	void Connect(const boost::asio::ip::tcp::resolver::results_type& endpoint);
	void Close(bool isForce = false);

public:
	virtual void Write(const CorePacket& packet) override;

private:
	virtual void Write(void) override;
	virtual void ReadHeader(void) override;
	virtual void ReadBody(void) override;

public:
	int32_t GetToken(void) const;
	void SetToken(const int32_t token);

private:
	int32_t token = 0;

private:
	boost::asio::io_context& ioContext;

private:
	CoreClient* client;
};