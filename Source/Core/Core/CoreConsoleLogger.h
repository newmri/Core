#pragma once

class CoreConsoleLogger : public CoreLogger
{
	OVERRIDE_OBJECT(CoreConsoleLogger)

public:
	virtual void Log(std::string_view logMessage) override;
	virtual void Log(const LogType logType, std::string_view logMessage) override;
};