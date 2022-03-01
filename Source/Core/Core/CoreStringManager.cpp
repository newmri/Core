#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreStringManager)

void CoreStringManager::Init(void)
{
	setlocale(LC_ALL, "");
}

void CoreStringManager::Release(void)
{
	GetInstance().~CoreStringManager();
}

std::string CoreStringManager::ReplaceAll(std::string_view source, std::string_view from, std::string_view to)
{
	std::string newString;
	newString.reserve(source.length());

	std::string::size_type findPos = 0, lastPos = 0;

	while (std::string::npos != (findPos = source.find(from, lastPos)))
	{
		newString.append(source, lastPos, findPos - lastPos);
		newString += to;
		lastPos = findPos + from.length();
	}

	newString += source.substr(lastPos);

	return newString;
}

bool CoreStringManager::IsValidEnglish(std::string_view str, const int8_t min, const int8_t max)
{
	if (!IsValidLen(str, min, max))
		return false;

	static std::regex pattern("^[a-z0-9A-Z]+$");
	return std::regex_match(str.data(), pattern);
}

bool CoreStringManager::IsValidEnglish(std::wstring_view str, const int8_t min, const int8_t max)
{
	if (!IsValidLen(str, min, max))
		return false;

	static std::wregex pattern(L"^[a-z0-9A-Z]+$");
	return std::regex_match(str.data(), pattern);
}

bool CoreStringManager::IsValidLanguage(std::wstring_view str, const int8_t min, const int8_t max)
{
	// ÃßÈÄ ¾ğ¾î°¡ Ãß°¡ µÉ½Ã ±×¿¡ ¸Â´Â ÇÔ¼ö È£Ãâ
	return IsValidKorean(str, min, max);
}

bool CoreStringManager::IsValidKorean(std::wstring_view str, const int8_t min, const int8_t max)
{
	if (!IsValidLen(str, min, max))
		return false;

	static std::wregex pattern(L"^[°¡-ÆRa-z0-9A-Z]+$");
	return std::regex_match(str.data(), pattern);
}

bool CoreStringManager::IsValidLen(std::string_view str, const int8_t min, const int8_t max)
{
	size_t len = str.length();
	return (len >= min && len <= max);
}

bool CoreStringManager::IsValidLen(std::wstring_view str, const int8_t min, const int8_t max)
{
	size_t len = str.length();
	return (len >= min && len <= max);
}

std::wstring CoreStringManager::Widen(std::string_view source)
{
	return CA2W(source.data(), CP_UTF8).m_psz;
}

std::string CoreStringManager::Narrow(std::wstring_view source)
{
	return CW2A(source.data(), CP_UTF8).m_psz;
}