#pragma once

class GamePacketFunc
{
public:
	void SC_LOGIN_RES(CoreServerSession& session, const void* data);
	void SC_PING_REQ(CoreServerSession& session, const void* data);
	void SC_SPAWN_PLAYER_NOTI(CoreServerSession& session, const void* data);
	void SC_DESPAWN_OBJECT_NOTI(CoreServerSession& session, const void* data);
	void SC_MOVE_RES(CoreServerSession& session, const void* data);
	void SC_SET_STATE_RES(CoreServerSession& session, const void* data);
	void SC_USE_SKILL_RES(CoreServerSession& session, const void* data);
	void SC_GET_DAMAGE_NOTI(CoreServerSession& session, const void* data);
	void SC_REVIVE_RES(CoreServerSession& session, const void* data);
	void SC_SPAWN_PROJECTILE_NOTI(CoreServerSession& session, const void* data);
};