#pragma once

class CoreGameObj : public CoreObj
{
	OVERRIDE_OBJ(CoreGameObj)

public:
	virtual void Update() CORE_PURE;
};