#pragma once

class Projectile : public Object, public CoreMemoryPoolObj<Projectile, CORE_BIG_SIZE>
{
	OVERRIDE_GAME_OBJECT(Projectile)

public:
	Projectile(const std::shared_ptr<ProjectileSkill> owner, const Info::ObjectInfoWithPosT& objectInfoWithPos);

public:
	virtual void MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet) override;

private:
	std::tuple<bool, std::shared_ptr<Object>> Move(void);

protected:
	virtual void DoDamage(std::shared_ptr<Object> target);

protected:
	std::shared_ptr<ProjectileSkill> owner;

protected:
	float moveSpeed;
	TIME_VALUE moveTick;
};