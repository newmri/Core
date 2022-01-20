#pragma once

#define STRING_MANAGER GET_INSTANCE(CoreStringManager)

class CoreStringManager
{
	DECLARE_SINGLETON(CoreStringManager)

public:
	std::string ReplaceAll(std::string_view source, std::string_view from, std::string_view to);
};