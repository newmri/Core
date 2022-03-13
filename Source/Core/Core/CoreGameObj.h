#pragma once

class CoreGameObj : public CoreUIDObj
{
	OVERRIDE_OBJ_DELETE_DEFAULT_CONSTRUCTOR(CoreGameObj)

public:
	CoreGameObj(const int64_t uid, const uint8_t ObjType);

public:
	uint8_t GetObjType(void);

public:
	virtual void Update() CORE_PURE;

protected:
	uint8_t objType = 0;
};