#pragma once

class CoreGameObject : public CoreObject
{
	OVERRIDE_OBJECT(CoreGameObject)

public:
	virtual void Update(void) CORE_PURE;
	virtual void Clear(void) CORE_PURE;
};