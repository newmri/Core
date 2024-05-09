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
	Object::Init();
}

void Projectile::Update(void)
{
	Object::Update();
}

void Projectile::Clear(void)
{
	Object::Clear();
}

void Projectile::DoDamage(std::shared_ptr<Object> target)
{
	
}
