#pragma once

class CoreGameObject : public CoreObject
{
	OVERRIDE_OBJECT(CoreGameObject)

public:
	virtual void Update() CORE_PURE;
};