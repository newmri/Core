#pragma once

enum class LogType : size_t
{
	LOG_INFO,
	LOG_ERROR,
	LOG_TYPE_NUM
};

class CoreLogger : public CoreObj
{
	OVERRIDE_OBJ(CoreLogger)

private:
	void MakeLog(const LogType logType);

protected:
	void MakeLog(const LogType logType, std::string_view logMessage);

public:
	std::string MakeLog(const LogType logType, std::string_view logMessage, std::string_view file, const char* function, const size_t line);

public:
	virtual void Log(std::string_view logMessage) = 0;
	virtual void Log(const LogType logType, std::string_view logMessage) = 0;

protected:
	std::string delimiter = "\t";
	std::string log;

private:
	std::string logHeader[static_cast<size_t>(LogType::LOG_TYPE_NUM)] = { "[LOG_INFO]", "[LOG_ERROR]" };

	std::ostringstream output;
	std::string dummyStr;
};