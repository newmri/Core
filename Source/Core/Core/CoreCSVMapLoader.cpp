#include "CoreInclude.h"

CoreCSVMapLoader::CoreCSVMapLoader()
{
	Init();
}

CoreCSVMapLoader::~CoreCSVMapLoader()
{

}

void CoreCSVMapLoader::Init(void)
{
	CoreCSVLoader::Init();
}

void CoreCSVMapLoader::PreLoad(std::string_view filePath, char*& out)
{
	CoreLoader::PreLoad(filePath, out);

	std::string str;
	str.reserve(CORE_BIG_SIZE);

	int32_t minX = 0, minY = 0, maxX = 0, maxY = 0;

	getline(this->fileStream, str);
	Parse(str, minX);

	getline(this->fileStream, str);
	Parse(str, minY);

	getline(this->fileStream, str);
	Parse(str, maxX);

	getline(this->fileStream, str);
	Parse(str, maxY);

	CORE_LOG.Log(LogType::LOG_INFO, "MinX: " + TO_STR(minX) + " minY: " + TO_STR(minY) + 
		" maxX: " + TO_STR(maxX) + " maxY: " + TO_STR(maxY));

	this->columns = (size_t)maxX - minX + 1;
	this->rows = (size_t)maxY - minY + 1;

	std::string strForParse;
	for (size_t i = 0; i < this->columns; ++i)
	{
		strForParse.append(UINT8_NAME);
		strForParse.append(",");
	}

	strForParse.erase(strForParse.length() - 1);
	ParseDataTypesAndCalRowSize(strForParse);

	size_t minMaxSize = sizeof(minX) * 4; // minX, minY, maxX, maxY

	out = new char[minMaxSize + (this->rowSize * (this->rows))]{};

	memcpy_s(out + this->offset, SIZE_OF_INT32, &minX, SIZE_OF_INT32); this->offset += SIZE_OF_INT32;
	memcpy_s(out + this->offset, SIZE_OF_INT32, &minY, SIZE_OF_INT32); this->offset += SIZE_OF_INT32;
	memcpy_s(out + this->offset, SIZE_OF_INT32, &maxX, SIZE_OF_INT32); this->offset += SIZE_OF_INT32;
	memcpy_s(out + this->offset, SIZE_OF_INT32, &maxY, SIZE_OF_INT32); this->offset += SIZE_OF_INT32;
}

void CoreCSVMapLoader::Load(char*& out)
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

			Parse(strForParse, out + this->offset + (this->rowSize * rows));

			strForParse = "";
			strForParse.append(str, newLinePos + 1);

			++rows;
		}

		else strForParse += str;
	}

	Close();
}

size_t CoreCSVMapLoader::Load(std::string_view filePath, char*& out)
{
	PreLoad(filePath, out);

	Load(out);

	LogLoadingEnd();

	return this->rows;
}
