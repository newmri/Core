#include "Include.h"

Projectile::Projectile(const std::shared_ptr<Creature> owner, const Info::ObjectInfoT& objectInfo) : owner(owner), Object(objectInfo)
{
	CORE_TIME_DELEGATE_MANAGER.Push(
		CoreTimeDelegate<>(std::bind(&Projectile::Update, this), 10));
}

Projectile::~Projectile()
{

}

void Projectile::Init(void)
{

}

void Projectile::MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
	auto objectInfo = GetObjectInfo();

	PACKET_SEND_MANAGER.builder.Clear();
	auto packedObjectInfo = Info::ObjectInfo::Pack(PACKET_SEND_MANAGER.builder, &objectInfo);
	auto message = GamePacket::CreateSC_SPAWN_PROJECTILE_NOTI(PACKET_SEND_MANAGER.builder, packedObjectInfo, this->moveSpeed);

	packetType = GamePacket::Packet_SC_SPAWN_PROJECTILE_NOTI;
	packet = message.Union();
}

void Projectile::Update(void)
{
	if (!Move())
	{
		OBJECT_MANAGER.RemoveProjectile(this->GetOID());
		return;
	}

	CORE_TIME_DELEGATE_MANAGER.Push(
		CoreTimeDelegate<>(std::bind(&Projectile::Update, this), SEC));
}

bool Projectile::Move(void)
{
	NativeInfo::Vec2Int destPos = GetPos().GetFrontPos(GetDir(), static_cast<uint8_t>(moveSpeed));
	return ZONE_MANAGER.Move(shared_from_this(), destPos, false, true);
}