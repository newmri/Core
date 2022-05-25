#include "Include.h"

Arrow::Arrow(const std::shared_ptr<ProjectileSkill> owner, const Info::ObjectInfoWithPosT& objectInfoWithPos) : Projectile(owner, objectInfoWithPos)
{
	
}

Arrow::~Arrow()
{

}

void Arrow::Init(void)
{

}

void Arrow::Update(void)
{
	Projectile::Update();
}

void Arrow::Clear(void)
{
	Projectile::Clear();
}

void Arrow::DoDamage(std::shared_ptr<Object> target)
{
	PACKET_SEND_MANAGER.Clear();
	std::vector<flatbuffers::Offset<GamePacket::DamageInfo>> sendList;
	this->owner->DoDamage(target, sendList);

	if (!sendList.empty())
	{
		auto message = GamePacket::CreateSC_GET_DAMAGE_NOTI(PACKET_SEND_MANAGER.builder, PACKET_SEND_MANAGER.builder.CreateVector(sendList));
		ZONE_MANAGER.SendAll(this->owner->GetMapID(), GamePacket::Packet_SC_GET_DAMAGE_NOTI, message.Union(), this->owner->GetPos());
	}
}
