#include "CoreInclude.h"

CoreCSVLoader::CoreCSVLoader()
{
	Init();
}

CoreCSVLoader::~CoreCSVLoader()
{

}

void CoreCSVLoader::Init(void)
{
	this->delimiter = ",";
}

void CoreCSVLoader::PreLoad(std::string_view filePath, char*& out)
{
	CoreLoader::PreLoad(filePath, out);

	std::string str;
	str.reserve(CORE_BIG_SIZE);

	std::string log;
	log.reserve(CORE_BIG_SIZE);

	std::string strForParse;
	strForParse.reserve(CORE_BIG_SIZE);

	std::string::size_type newLinePos = 0;

	while (!this->fileStream.eof())
	{
		getline(this->fileStream, str, this->delimiter.front());

		str.append(this->delimiter);

		newLinePos = str.find('\n');

		if (0 == this->rows)
			++this->columns;

		if (std::string::npos != newLinePos)
		{
			log.append(str, 0, newLinePos);

			strForParse = log;

			strForParse.append("\n,");

			if (0 == rows)
			{
				ParseDataTypesAndCalRowSize(strForParse);
			}

			CORE_LOG.Log(LogType::LOG_INFO, log);

			log = "";
			log.append(str, newLinePos + 1);

			++this->rows;
		}

		else log += str;
	}

	this->rows -= 1;

	out = new char[this->rowSize * (this->rows)]{};

	Rewind();
}

void CoreCSVLoader::Load(char*& out)
{
	std::string str;
	str.reserve(CORE_BIG_SIZE);

	std::string strForParse;
	strForParse.reserve(CORE_BIG_SIZE);

	std::string::size_type newLinePos = 0;

	size_t rows = 0;

	while (!this->fileStream.eof())
	{
		getline(this->fileStream, str, this->delimiter.front());

		str.append(this->delimiter);

		newLinePos = str.find('\n');

		if (std::string::npos != newLinePos)
		{
			strForParse.append(str, 0, newLinePos);
			strForParse.append("\n,");

			if (0 != rows)
			{
				Parse(strForParse, out + (this->rowSize * (rows - 1)));
			}

			strForParse = "";
			strForParse.append(str, newLinePos + 1);

			++rows;
		}

		else strForParse += str;
	}

	Close();
}

size_t CoreCSVLoader::Load(std::string_view filePath, char*& out)
{
	PreLoad(filePath, out);

	Load(out);

	LogLoadingEnd();

	return this->rows;
}
