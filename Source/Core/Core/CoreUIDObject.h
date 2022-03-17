#pragma once

class CoreUIDObject : public CoreObject
{
	OVERRIDE_OBJECT_DELETE_DEFAULT_CONSTRUCTOR(CoreUIDObject)

public:
	CoreUIDObject(const int64_t uid);

public:
	int64_t GetUID(void) const;

protected:
	int64_t uid = 0;
};