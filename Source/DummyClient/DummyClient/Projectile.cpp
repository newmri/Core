#include "Include.h"

Projectile::Projectile(const std::shared_ptr<ProjectileSkill> owner, const Info::ObjectInfoT& objectInfo) : 
	owner(owner), Object(objectInfo), moveSpeed(owner->GetSpeed())
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

	GAME_PACKET_SEND_MANAGER.Clear();
	auto packedObjectInfo = Info::ObjectInfo::Pack(GAME_PACKET_SEND_MANAGER.builder, &objectInfo);
	auto message = GamePacket::CreateSC_SPAWN_PROJECTILE_NOTI(GAME_PACKET_SEND_MANAGER.builder, packedObjectInfo, this->owner->GetType(), this->moveSpeed);

	packetType = GamePacket::Packet_SC_SPAWN_PROJECTILE_NOTI;
	packet = message.Union();
}

void Projectile::Update(void)
{

}

void Projectile::DoDamage(std::shared_ptr<Object> target)
{
	
}
