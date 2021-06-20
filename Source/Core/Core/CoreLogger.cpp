#include "CoreInclude.h"

CoreLogger::CoreLogger()
{
	Init();
}

CoreLogger::~CoreLogger()
{

}

void CoreLogger::Init(void)
{
	this->log = CORE_DUMMY_MANAGER.GetDummyString();
}

void CoreLogger::MakeLog(const LogType logType)
{
	this->log = this->logHeader[static_cast<size_t>(logType)] + this->delimiter;
}

void CoreLogger::MakeLog(const LogType logType, std::string_view logMessage)
{
	MakeLog(logType);
	this->log.append(logMessage);
}

std::string CoreLogger::MakeLog(const LogType logType, std::string_view logMessage, std::string_view file, const char* function, const size_t line)
{
	MakeLog(logType, logMessage);

	this->output.str(CORE_DUMMY_MANAGER.GetDummyString().data());

	this->output << this->log;
	this->output << file << this->delimiter;
	this->output << function << this->delimiter;
	this->output << line << this->delimiter;

	return this->output.str();
}