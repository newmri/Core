#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreStringManager)

void CoreStringManager::Init(void)
{

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

std::wstring CoreStringManager::Widen(std::string_view source)
{
	std::wostringstream wstm;
	const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());
	for (size_t i = 0; i < source.size(); ++i)
		wstm << ctfacet.widen(source[i]);

	return wstm.str();
}

std::string CoreStringManager::Narrow(std::wstring_view source)
{
	std::ostringstream stm;

	const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(stm.getloc());

	for (size_t i = 0; i < source.size(); ++i)
		stm << ctfacet.narrow(source[i], 0);

	return stm.str();
}