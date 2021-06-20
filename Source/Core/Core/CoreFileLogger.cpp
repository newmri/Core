#include "CoreInclude.h"

CoreFileLogger::CoreFileLogger()
{
    Init();
}

CoreFileLogger::~CoreFileLogger()
{
    this->output.close();
}

void CoreFileLogger::MakeFullPath(void)
{
    this->currFullPath = this->filePath + CORE_TIME_MANAGER.GetDate() + this->fileExtension;
}

void CoreFileLogger::Init(void)
{
    _mkdir(filePath.c_str());
    MakeFullPath();
}

void CoreFileLogger::UpdateFile(void)
{
    MakeFullPath();

    if (this->prevFullPath != this->currFullPath)
    {
        this->prevFullPath = this->currFullPath;

        if (this->output.is_open())
        {
            this->output.close();
        }

        this->output.open(this->currFullPath, std::ios::app);

        if (!this->output.is_open())
        {
            throw std::runtime_error(CORE_LOG.MakeLog(LogType::LOG_ERROR, this->currFullPath, __FILE__, __FUNCTION__, __LINE__));
        }
    }
}

void CoreFileLogger::Log(std::string_view logMessage)
{
    UpdateFile();

    this->output << CORE_TIME_MANAGER.GetTime() << this->delimiter << logMessage << std::endl;
}

void CoreFileLogger::Log(const LogType logType, std::string_view logMessage)
{
    MakeLog(logType, logMessage);

    UpdateFile();

    this->output << CORE_TIME_MANAGER.GetTime() << this->delimiter << this->log << std::endl;
}