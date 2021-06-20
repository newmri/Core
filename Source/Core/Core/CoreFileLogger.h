#pragma once

class CoreFileLogger : public CoreLogger
{
	OVERRIDE_OBJ(CoreFileLogger)

private:
	void MakeFullPath(void);
	void UpdateFile(void);

public:
	virtual void Log(std::string_view logMessage) override;
	virtual void Log(const LogType logType, std::string_view logMessage) override;

private:
	std::string prevFullPath, currFullPath;
	std::string filePath = ".\\DebugLog\\";
	std::string fileExtension = ".log";
	std::ofstream output;
};