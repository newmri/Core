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