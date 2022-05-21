#include "Include.h"

Object::Object(const Info::ObjectInfoWithPosT& objectInfoWithPos) : objectInfoWithPos(objectInfoWithPos)
{
	Init();
}

Object::~Object()
{

}

void Object::Init(void)
{

}

void Object::Update(void)
{

}

Info::ObjectInfoWithPosT Object::GetObjectInfoWithPos(void)
{
	READ_LOCK(this->infoMutex);
	return this->objectInfoWithPos;
}

NativeInfo::ObjectInfo Object::GetObjectInfo(void)
{
	return this->objectInfoWithPos.object_info;
}

Info::ObjectInfo Object::GetPackedObjectInfo(void)
{
	return flatbuffers::PackObjectInfo(GetObjectInfo());
}

NativeInfo::PositionInfo Object::GetPosInfo(void)
{
	READ_LOCK(this->infoMutex);
	return this->objectInfoWithPos.pos_info;
}

Define::ObjectType Object::GetObjectType(void) const
{
	return this->objectInfoWithPos.object_info.objectType;
}

int64_t Object::GetOID(void) const
{
	return this->objectInfoWithPos.object_info.oid;
}

int32_t Object::GetMapID(void)
{
	READ_LOCK(this->infoMutex);
	return this->objectInfoWithPos.pos_info.mapID;
}

NativeInfo::Vec2Int Object::GetPos(void)
{
	READ_LOCK(this->infoMutex);
	return GetPosWithNoLock();
}

Define::Dir Object::GetDir(void)
{
	READ_LOCK(this->infoMutex);
	return this->objectInfoWithPos.pos_info.moveDir;
}

bool Object::IsDead(void)
{
	return (IS_SAME(Define::ObjectState_DEAD, GetState()));
}

Define::ObjectState Object::GetState(void)
{
	READ_LOCK(this->infoMutex);
	return GetStateWithNoLock();
}

NativeInfo::Vec2Int Object::GetPosWithNoLock(void) const
{
	return this->objectInfoWithPos.pos_info.pos;
}

Define::ObjectState Object::GetStateWithNoLock(void) const
{
	return this->objectInfoWithPos.pos_info.state;
}

void Object::SetObjectInfoWithPos(const Info::ObjectInfoWithPosT& objectInfoWithPos)
{
	WRITE_LOCK(this->infoMutex);
	this->objectInfoWithPos = objectInfoWithPos;
}

void Object::SetMove(const Define::ObjectState state, const NativeInfo::Vec2Int& destPos)
{
	WRITE_LOCK(this->infoMutex);
	SetStateWithNoLock(state);
	SetDirectionWithNoLock(destPos);
	SetPosWithNoLock(destPos);
}

void Object::SetState(const Define::ObjectState state)
{
	WRITE_LOCK(this->infoMutex);
	SetStateWithNoLock(state);
}

void Object::SetDirection(const NativeInfo::Vec2Int& destPos)
{
	WRITE_LOCK(this->infoMutex);
	SetDirectionWithNoLock(destPos);
}

void Object::SetPos(const NativeInfo::Vec2Int& pos)
{
	WRITE_LOCK(this->infoMutex);
	SetPosWithNoLock(pos);
}

void Object::SetStateWithNoLock(const Define::ObjectState state)
{
	this->objectInfoWithPos.pos_info.state = state;
}

void Object::SetDirectionWithNoLock(const NativeInfo::Vec2Int& destPos)
{
	this->objectInfoWithPos.pos_info.moveDir = GetPosWithNoLock().GetDirection(destPos);
}

void Object::SetPosWithNoLock(const NativeInfo::Vec2Int& pos)
{
	this->objectInfoWithPos.pos_info.pos = pos;
}

void Object::MakeMovePacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
	GAME_PACKET_SEND_MANAGER.Clear();
	auto objectInfoWithPos = GetObjectInfoWithPos();
	auto packedObjectInfo = Info::ObjectInfoWithPos::Pack(GAME_PACKET_SEND_MANAGER.builder, &objectInfoWithPos);
	auto message = GamePacket::CreateSC_MOVE_RES(GAME_PACKET_SEND_MANAGER.builder, packedObjectInfo);
	packetType = GamePacket::Packet_SC_MOVE_RES;
	packet = message.Union();
}

bool Object::OnGetDamage(const GamePacket::DamageInfoT& damageInfo)
{
	return false;
}