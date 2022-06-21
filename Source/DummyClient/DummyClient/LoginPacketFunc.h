#pragma once

class LoginPacketFunc
{
public:
	void SC_LOGIN_RES(std::shared_ptr<CoreServerSession> session, const void* data);
	void SC_PING_REQ(std::shared_ptr<CoreServerSession> session, const void* data);
	void SC_CREATE_CHARACTER_RES(std::shared_ptr<CoreServerSession> session, const void* data);
	void SC_DELETE_CHARACTER_RES(std::shared_ptr<CoreServerSession> session, const void* data);
};