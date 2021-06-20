#pragma once

#define CORE_DUMMY_MANAGER GET_INSTANCE(CoreDummyManager)

class CoreDummyManager
{
	DECLARE_SINGLETON(CoreDummyManager)

public:
	std::string_view GetDummyString(void) const;

private:
	std::string dummyString;
	static constexpr size_t defaultLogSize = 1024;
};