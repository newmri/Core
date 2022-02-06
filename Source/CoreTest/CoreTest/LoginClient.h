#pragma once

class LoginPacketHandler;

class LoginClient : public CoreClient
{
public:
	LoginClient();
	virtual ~LoginClient() override;

public:
	virtual void ProcessPacket(const uint8_t* data, size_t size) override;

private:
	LoginPacketHandler* handler;
};