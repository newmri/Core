#pragma once

class GamePacketFunc
{
public:
	void SC_LOGIN_RES(std::shared_ptr<CoreServerSession> session, const void* data);
	void SC_PING_REQ(std::shared_ptr<CoreServerSession> session, const void* data);
	void SC_SPAWN_PLAYER_NOTI(std::shared_ptr<CoreServerSession> session, const void* data);
	void SC_DESPAWN_OBJECT_NOTI(std::shared_ptr<CoreServerSession> session, const void* data);
	void SC_MOVE_RES(std::shared_ptr<CoreServerSession> session, const void* data);
	void SC_SET_STATE_RES(std::shared_ptr<CoreServerSession> session, const void* data);
	void SC_USE_SKILL_RES(std::shared_ptr<CoreServerSession> session, const void* data);
	void SC_GET_DAMAGE_NOTI(std::shared_ptr<CoreServerSession> session, const void* data);
	void SC_REVIVE_RES(std::shared_ptr<CoreServerSession> session, const void* data);
	void SC_SPAWN_PROJECTILE_NOTI(std::shared_ptr<CoreServerSession> session, const void* data);
};