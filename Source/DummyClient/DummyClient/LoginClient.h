#pragma once

class LoginClient : public CoreClient
{
public:
	LoginClient(const int64_t oid);
	virtual ~LoginClient() override;

private:
	void MakeAccountInfo(void);

public:
	bool Connect(void);

public:
	void SendLogout(void);
};