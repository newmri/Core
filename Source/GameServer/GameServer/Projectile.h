#pragma once

class Projectile : public Object, public CoreMemoryPoolObj<Projectile, CORE_BIG_SIZE>
{
	OVERRIDE_GAME_OBJECT(Projectile)

public:
	Projectile(const std::shared_ptr<Creature> owner, const Info::ObjectInfoT& objectInfo);

public:
	virtual void MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet);

private:
	bool Move(void);

protected:
	std::shared_ptr<Creature> owner;

protected:
	float moveSpeed = 15.0f;
};