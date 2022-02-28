#pragma once

#define STRING_MANAGER GET_INSTANCE(CoreStringManager)

class CoreStringManager
{
	DECLARE_SINGLETON(CoreStringManager)

public:
	std::string ReplaceAll(std::string_view source, std::string_view from, std::string_view to);

public:
	bool IsValidEnglish(std::string_view str, const int8_t min, const int8_t max);
	bool IsValidEnglish(std::wstring_view str, const int8_t min, const int8_t max);
	bool IsValidLanguage(std::wstring_view str, const int8_t min, const int8_t max);
	bool IsValidKorean(std::wstring_view str, const int8_t min, const int8_t max);

public:
	bool IsValidLen(std::string_view str, const int8_t min, const int8_t max);
	bool IsValidLen(std::wstring_view str, const int8_t min, const int8_t max);

public:
    std::wstring Widen(std::string_view source);
    std::string Narrow(std::wstring_view source);
};