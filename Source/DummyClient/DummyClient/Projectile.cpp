#include "Include.h"

Projectile::Projectile(const std::shared_ptr<ProjectileSkill> owner, const Info::ObjectInfoWithPosT& objectInfoWithPos) :
	owner(owner), Object(objectInfoWithPos), moveSpeed(owner->GetSpeed())
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
	auto packedObjectInfoWithPos = Info::ObjectInfoWithPos::Pack(GAME_PACKET_SEND_MANAGER.builder, &objectInfoWithPos);
	auto message = GamePacket::CreateSC_SPAWN_PROJECTILE_NOTI(GAME_PACKET_SEND_MANAGER.builder, packedObjectInfoWithPos, this->owner->GetType(), this->moveSpeed);

	packetType = GamePacket::Packet_SC_SPAWN_PROJECTILE_NOTI;
	packet = message.Union();
}

void Projectile::Update(void)
{

}

void Projectile::DoDamage(std::shared_ptr<Object> target)
{
	
}
