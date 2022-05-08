#pragma once

class LoginPacketFunc
{
public:
	void SC_LOGIN_RES(CoreServerSession& session, const void* data);
	void SC_PING_REQ(CoreServerSession& session, const void* data);
	void SC_CREATE_CHARACTER_RES(CoreServerSession& session, const void* data);
};