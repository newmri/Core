#include "CoreInclude.h"

CoreLoader::CoreLoader()
{
	Init();
}

CoreLoader::~CoreLoader()
{

}

void CoreLoader::Init(void)
{
	this->logStart.reserve(CORE_BIG_SIZE);
	this->logEnd.reserve(CORE_BIG_SIZE);
}

void CoreLoader::PreLoad(std::string_view filePath, char*& out)
{
	this->filePath = filePath;
	this->dataTypes.reset();
	this->rows = 0;
	this->columns = 0;
	this->rowSize = 0;

	Open();

	LogLoadingStart();
}

void CoreLoader::Open()
{
	this->fileStream.open(std::string(this->filePath).c_str(), std::ios::in);

	if (!this->fileStream.is_open())
	{
		throw std::invalid_argument(CORE_LOG.MakeLog(LogType::LOG_ERROR, filePath, __FILE__, __FUNCTION__, __LINE__));
	}
}

void CoreLoader::Rewind(void)
{
	this->fileStream.seekg(0, std::ios::beg);
}

void CoreLoader::Close(void)
{
	this->fileStream.close();
}

void CoreLoader::LogLoadingStart(void)
{
	this->logStart = "[";
	this->logStart.append(this->filePath);
	this->logStart.append(" is loading...]");

	CORE_LOG.Log(LogType::LOG_INFO, this->logStart);
}

void CoreLoader::LogLoadingEnd(void)
{
	this->logEnd = "[";

	this->logEnd.append(this->filePath);
	this->logEnd.append(" is loaded...]");

	CORE_LOG.Log(LogType::LOG_INFO, this->logEnd);
}

void CoreLoader::Parse(std::string& in, char* out)
{
	for (size_t i = 0; i < this->columns; ++i)
	{
		Parse(in, i, out);
		out += CORE_DATA_TYPE_MANAGER.GetSizeOfType(this->dataTypes[i]);
	}
}

void CoreLoader::Parse(std::string& in, const size_t dataTypeIndex, char* out)
{
#if _DEBUG
	unsigned int hashCode = HashCode(this->dataTypes[dataTypeIndex].c_str());

	if (hashCode == HashCode(INT16_NAME))
	{
		Parse(in, CHAR_TO_INT16_REF out);
	}

	else if (hashCode == HashCode(INT32_NAME))
	{
		Parse(in, CHAR_TO_INT32_REF out);
	}

	else if (hashCode == HashCode(INT64_NAME))
	{
		Parse(in, CHAR_TO_INT64_REF out);
	}

	else if (hashCode == HashCode(SIZE_T_NAME))
	{
		Parse(in, CHAR_TO_SIZE_T_REF out);
	}

	else if (hashCode == HashCode(FLOAT_NAME))
	{
		Parse(in, CHAR_TO_FLOAT_REF out);
	}

	else if (hashCode == HashCode(DOUBLE_NAME))
	{
		Parse(in, CHAR_TO_DOUBLE_REF out);
	}

	else if (hashCode == HashCode(STRING_NAME))
	{
		Parse(in, CHAR_TO_STRING_REF out);
	}
#else
	switch (HashCode(this->dataTypes[dataTypeIndex].c_str()))
	{
	case HashCode(INT16_NAME):
		Parse(in, CHAR_TO_INT16_REF out);
		break;
	case HashCode(INT32_NAME):
		Parse(in, CHAR_TO_INT32_REF out);
		break;
	case HashCode(INT64_NAME):
		Parse(in, CHAR_TO_INT64_REF out);
		break;
	case HashCode(SIZE_T_NAME):
		Parse(in, CHAR_TO_SIZE_T_REF out);
		break;
	case HashCode(FLOAT_NAME):
		Parse(in, CHAR_TO_FLOAT_REF out);
		break;
	case HashCode(DOUBLE_NAME):
		Parse(in, CHAR_TO_DOUBLE_REF out);
		break;
	case HashCode(STRING_NAME):
		Parse(in, CHAR_TO_STRING_REF out);
		break;
	}
#endif // _DEBUG
}

std::string CoreLoader::Parse(std::string& in)
{
	std::string str;
	str.reserve(CORE_BIG_SIZE);
	std::string::size_type findPos = 0;

	findPos = in.find(this->delimiter);

	for (std::string::size_type pos = 0; pos < findPos; ++pos)
	{
		str.push_back(in[pos]);
	}

	in.erase(0, findPos + 1);

	return str;
}

void CoreLoader::Parse(std::string& in, int16_t& out)
{
	out = stoi(Parse(in));
}

void CoreLoader::Parse(std::string& in, int32_t& out)
{
	out = stoi(Parse(in));
}

void CoreLoader::Parse(std::string& in, int64_t& out)
{
	out = stoll(Parse(in));
}

void CoreLoader::Parse(std::string& in, size_t& out)
{
	out = stoull(Parse(in));
}

void CoreLoader::Parse(std::string& in, float_t& out)
{
	out = stof(Parse(in));
}

void CoreLoader::Parse(std::string& in, double_t& out)
{
	out = stod(Parse(in));
}

void CoreLoader::ParseDataTypesAndCalRowSize(std::string& strForParse)
{
	this->dataTypes = std::make_unique<std::string[]>(this->columns);

	for (size_t i = 0; i < this->columns; ++i)
	{
		this->dataTypes[i] = Parse(strForParse);
	}

	size_t lastIndex = this->columns - 1;

	this->dataTypes[lastIndex] = STRING_MANAGER.ReplaceAll(this->dataTypes[lastIndex], "\n", "");

	this->rowSize = 0;

	for (size_t i = 0; i < this->columns; ++i)
	{
		this->rowSize += CORE_DATA_TYPE_MANAGER.GetSizeOfType(this->dataTypes[i]);
	}
}

void CoreLoader::Parse(std::string& in, std::string& out)
{
	out = Parse(in);
}

