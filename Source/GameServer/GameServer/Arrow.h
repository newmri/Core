#pragma once

class Arrow : public Projectile
{
	OVERRIDE_GAME_OBJECT(Arrow)

public:
	Arrow(const std::shared_ptr<ProjectileSkill> owner, const Info::ObjectInfoT& objectInfo);

protected:
	virtual void DoDamage(std::shared_ptr<Object> target) override;
};