#pragma once

class LoginPacketHandler;

class LoginClient : public CoreClient
{
public:
	LoginClient(const int64_t oid);
	virtual ~LoginClient() override;

private:
	void MakeAccountInfo(void);

public:
	void Connect(void);

public:
	virtual void OnConnected(void) override;
	virtual void OnDisconnected(void) override;
	virtual void ProcessPacket(const uint8_t* data, size_t size) override;

public:
	void SendLogout(void);

public:
	std::string_view GetID(void);
	std::string_view GetPassword(void);

private:
	std::string id;
	std::string password;

private:
	LoginPacketHandler* handler;
};