#pragma once

class CoreCharacter : public CoreUIDObject, public CoreMemoryPoolObj<CoreCharacter, CORE_BIG_SIZE>, public std::enable_shared_from_this<CoreCharacter>
{
	OVERRIDE_OBJECT_DELETE_DEFAULT_CONSTRUCTOR(CoreCharacter)

public:
	CoreCharacter(const int64_t& accountUID, const int64_t& uid);

public:
	int64_t GetAccountUID(void);

protected:
	int64_t accountUID;
};