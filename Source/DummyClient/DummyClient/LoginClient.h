#pragma once

class LoginPacketHandler;

class LoginClient : public CoreClient
{
public:
	LoginClient(const int64_t uid);
	virtual ~LoginClient() override;

public:
	virtual void OnConnected(void) override;
	virtual void OnDisconnected(void) override;
	virtual void ProcessPacket(const uint8_t* data, size_t size) override;

private:
	void Signup(void);

private:
	LoginPacketHandler* handler;
};