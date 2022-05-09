#pragma once

class LoginPacketHandler;

class LoginClient : public CoreClient
{
public:
	LoginClient(const int64_t uid);
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
	std::string_view GetID(void);
	std::string_view GetPassword(void);

public:
	int64_t GetAccountUID(void) const;
	int32_t GetToken(void) const;

public:
	void SetAccountUID(const int64_t accountUID);
	void SetToken(const int32_t token);

private:
	std::string id;
	std::string password;

private:
	int64_t accountUID = 0;
	int32_t token = 0;

private:
	LoginPacketHandler* handler;
};