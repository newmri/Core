#pragma once

class GamePacketFunc
{
public:
	void CS_LOGIN_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
	static void SC_PING_REQ(std::shared_ptr<CoreClientSession> session);
	void CS_PING_RES(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_LOGOUT_NOTI(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_MOVE_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_SET_STATE_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_USE_SKILL_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_REVIVE_REQ(std::shared_ptr<CoreClientSession> session, const void* data);;
	void CS_EQUIP_GEAR_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_UNEQUIP_GEAR_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_NORMAL_CHAT_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_ADD_STAT_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_USE_ITEM_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
};