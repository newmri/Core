#include "Include.h"

Projectile::Projectile(const std::shared_ptr<ProjectileSkill> owner, const Info::ObjectInfoWithPosT& objectInfoWithPos) :
	owner(owner), Object(objectInfoWithPos), moveSpeed(owner->GetSpeed()), moveTick(static_cast<TIME_VALUE>(SEC / this->moveSpeed))
{

}

Projectile::~Projectile()
{

}

void Projectile::Init(void)
{
	Object::Init();
}

void Projectile::Clear(void)
{
	Object::Clear();
}

void Projectile::PushMove(void)
{
	CORE_TIME_DELEGATE_MANAGER.Push(
		CoreTimeDelegate<>(std::bind(&Projectile::Update, Object::downcasted_shared_from_this<Projectile>()), this->moveTick));
}

void Projectile::MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
	auto objectInfoWithPos = GetObjectInfoWithPos();

	PACKET_SEND_MANAGER.Clear();
	auto packedObjectInfoWithPos = Info::ObjectInfoWithPos::Pack(PACKET_SEND_MANAGER.builder, &objectInfoWithPos);
	auto message = GamePacket::CreateSC_SPAWN_PROJECTILE_NOTI(PACKET_SEND_MANAGER.builder, packedObjectInfoWithPos, this->owner->GetType(), this->moveSpeed);

	packetType = GamePacket::Packet_SC_SPAWN_PROJECTILE_NOTI;
	packet = message.Union();
}

void Projectile::Update(void)
{
	if (auto [moved, object] = Move(); IS_SAME(false, moved))
	{
		if(object)
			DoDamage(object);

		OBJECT_MANAGER.RemoveProjectile(this->GetOID());
		return;
	}

	PushMove();
}

std::tuple<bool, std::shared_ptr<Object>> Projectile::Move(void)
{
	NativeInfo::Vec2Int destPos = GetPos().GetFrontPos(GetDir());
	return ZONE_MANAGER.Move(shared_from_this(), destPos, true, false, true);
}

void Projectile::DoDamage(std::shared_ptr<Object> target)
{
	
}
