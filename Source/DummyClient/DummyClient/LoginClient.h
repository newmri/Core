#pragma once

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
	void SendLogout(void);

private:
	boost::thread_group m_group;
};