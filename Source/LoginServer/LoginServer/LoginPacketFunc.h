#pragma once

class LoginPacketFunc
{
public:
	void CS_LOGIN_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
	static void SC_PING_REQ(std::shared_ptr<CoreClientSession> session);
	void CS_PING_RES(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_LOGOUT_NOTI(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_CREATE_CHARACTER_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_DELETE_CHARACTER_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
};