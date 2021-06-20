#include "CoreInclude.h"

CoreConsoleLogger::CoreConsoleLogger()
{
	Init();
}

CoreConsoleLogger::~CoreConsoleLogger()
{

}

void CoreConsoleLogger::Init(void)
{

}

void CoreConsoleLogger::Log(std::string_view logMessage)
{
	std::cout << CORE_TIME_MANAGER.GetDateTime() << this->delimiter << logMessage << std::endl;
}

void CoreConsoleLogger::Log(const LogType logType, std::string_view logMessage)
{
	MakeLog(logType, logMessage);

	std::cout << CORE_TIME_MANAGER.GetDateTime() << this->delimiter << this->log << std::endl;
}