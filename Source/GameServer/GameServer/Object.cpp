#include "Include.h"

Object::Object(const Info::ObjectInfoT& objectInfo) : objectInfo(objectInfo)
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

Info::ObjectInfoT Object::GetObjectInfo(void)
{
	READ_LOCK(this->infoMutex);
	return this->objectInfo;
}

NativeInfo::PositionInfo Object::GetPosInfo(void)
{
	READ_LOCK(this->infoMutex);
	return this->objectInfo.pos_info;
}

Define::ObjectType Object::GetObjectType(void) const
{
	return this->objectInfo.object_type;
}

int64_t Object::GetOID(void) const
{
	return this->objectInfo.oid;
}

int32_t Object::GetMapID(void)
{
	READ_LOCK(this->infoMutex);
	return this->objectInfo.pos_info.mapID;
}

NativeInfo::Vec2Int Object::GetPos(void)
{
	READ_LOCK(this->infoMutex);
	return GetPosWithNoLock();
}

Define::Dir Object::GetDir(void)
{
	READ_LOCK(this->infoMutex);
	return this->objectInfo.pos_info.moveDir;
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
	return this->objectInfo.pos_info.pos;
}

Define::ObjectState Object::GetStateWithNoLock(void) const
{
	return this->objectInfo.pos_info.state;
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
	this->objectInfo.pos_info.state = state;
}

void Object::SetDirectionWithNoLock(const NativeInfo::Vec2Int& destPos)
{
	this->objectInfo.pos_info.moveDir = GetPosWithNoLock().GetDirection(destPos);
}

void Object::SetPosWithNoLock(const NativeInfo::Vec2Int& pos)
{
	this->objectInfo.pos_info.pos = pos;
}

void Object::MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
}

void Object::MakeMovePacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
	PACKET_SEND_MANAGER.builder.Clear();
	auto objectInfo = GetObjectInfo();
	auto packedObjectInfo = Info::ObjectInfo::Pack(PACKET_SEND_MANAGER.builder, &objectInfo);
	auto message = GamePacket::CreateSC_MOVE_RES(PACKET_SEND_MANAGER.builder, packedObjectInfo);
	packetType = GamePacket::Packet_SC_MOVE_RES;
	packet = message.Union();
}

bool Object::OnGetDamage(GamePacket::DamageInfoT& damageInfo, const Define::AbilityType defenceType)
{
	return false;
}