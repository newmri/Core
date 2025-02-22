#include "CoreInclude.h"

CoreFileLogger::CoreFileLogger()
{

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
    CoreLogger::Init();

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
            CORE_ERROR_LOG("currFullPath: {}", this->currFullPath);
            throw std::runtime_error("is_open fail");
        }
    }
}

void CoreFileLogger::Log(std::string_view logMessage)
{
    UpdateFile();

   this->output << CORE_TIME_MANAGER.GetTime() << this->delimiter << CoreUtil::GetThreadID() << this->delimiter << logMessage << std::endl;
}

void CoreFileLogger::Log(const LogType logType, std::string_view logMessage)
{
    MakeLog(logType, logMessage);

    UpdateFile();

   this->output << CORE_TIME_MANAGER.GetTime() << this->delimiter << CoreUtil::GetThreadID() << this->delimiter << this->log << std::endl;
}