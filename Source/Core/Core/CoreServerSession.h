#pragma once

class CoreServerSession : public CoreSession, public std::enable_shared_from_this<CoreServerSession>
{
public:
	CoreServerSession(const int64_t oid, boost::asio::io_context& ioContext);
	virtual ~CoreServerSession() override;

public:
	bool Connect(const boost::asio::ip::tcp::resolver::results_type& endpoint);
	virtual void Close(void) override;

public:
	virtual void Write(const CorePacket& packet) override;

private:
	virtual void Write(void) override;
	virtual void ReadHeader(void) override;
	virtual void ReadBody(void) override;

public:
	void SetOnConnectedFunc(std::function<void(std::shared_ptr<CoreServerSession>)> func);
	void SetOnDisconnectedFunc(std::function<void(std::shared_ptr<CoreServerSession>)> func);
	void SetProcessPacketFunc(std::function<void(std::shared_ptr<CoreServerSession>, uint8_t*, uint32_t)> func);

private:
	void OnDisconnectedFunc(void);

public:
	std::string_view GetAccountID(void);
	std::string_view GetAccountPassword(void);
	int64_t GetCharacterUID(void);

public:
	void SetAccountID(std::string_view accountID);
	void SetAccountPassword(std::string_view accountPassword);
	void SetCharacterUID(int64_t characterUID);

public:
	int32_t GetToken(void) const;
	void SetToken(const int32_t token);

private:
	std::string accountID;
	std::string accountPassword;

private:
	int64_t characterUID = 0;

private:
	int32_t token = 0;

private:
	boost::asio::io_context& ioContext;

private:
	std::function<void(std::shared_ptr<CoreServerSession>)> onConnectedFunc;
	std::function<void(std::shared_ptr<CoreServerSession>)> onDisconnectedFunc;
	std::function<void(std::shared_ptr<CoreServerSession>, uint8_t*, uint32_t)> processPacketFunc;
};